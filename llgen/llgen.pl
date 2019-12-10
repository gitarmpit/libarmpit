use warnings;
use strict;
use File::Basename;
use File::Spec;
use Getopt::Std;

my $output_dir         = "out";
my $log_file           = "log.txt";

# pdftotext or ghostscript
# Full or relative path if it is on PATH
my $pdftotext_path = "pdftotext.exe";
my $pdfotext_opts  = "-simple";

# check if memory file already exists
#my $file_exists_warning = 1;

# overwrite silently, manual changes will be lost
my $file_exists_warning = 0;

# lowest peripheral address to expect when parsing memory table
my $lowest_addr = "0x40000000";

# patterns
# Example: Address offset: 0x1FD0  (on a line by itself)
# our $address_offset = "Address offset:\\s+(0x[0-9A-F]{4})\\s*\$"
# more relaxed in case there are typos
my $address_offset = "Address offset:\\s+(\\S+)\\s*\$";

#########################################################

sub get_basename
{
    my @parts = split /[\/\\]/, $_[0];
    my $base  = $parts[$#parts];
    $base =~ s/^(.*)\.pdf$/$1/;
    if ( $base eq "" )
    {
        die "error parsing filename";
    }
    return $base;
}

sub prompt
{
    my ($query) = @_;    # take a prompt string as argument
    local $| = 1;        # activate autoflush to immediately show the prompt
    print $query;
    chomp( my $answer = <STDIN> );
    return $answer;
}

sub prompt_yn
{
    my ($query) = @_;
    my $answer;
    do
    {
        $answer = prompt("$query (y/n): ");
    } while ( $answer ne 'y' && $answer ne 'n' );
    return lc($answer) eq 'y';
}

my $pdf_fname;
############################################################################

# Parse the memory layout table and generate an intermediate memory file
# with peripheral names and their addresses
# Those can then be edited manually before the second pass which parse registers and attempt to match
# the register names with the peripheral names in the generated memory file
sub pass1
{
    my $pdf_base_name = get_basename($pdf_fname);
    my $txt_name      = "$pdf_base_name.txt";
    if ( !-e $txt_name )
    {
        print "Processing $pdf_fname and generating text file: $txt_name...\n";
        system("$pdftotext_path $pdfotext_opts \"$pdf_fname\" \"$txt_name\"");
        if ( $? != 0 )
        {
            die "Error running pdf converter command, check if $pdftotext_path exists";
        }
        print "Done!\n";
    }

    mkdir $output_dir unless -d $output_dir;

    my $fh;
    open( $fh, '<', $txt_name ) or die "Could not open file '$txt_name' $!";

    my $sep = "\r";
    if ( File::Spec->catfile( '', '' ) eq "\\" )
    {
        $sep = "\n|\r";
    }

    read $fh, my $file_content, -s $fh;
    my @lines = split /$sep/, $file_content;
    close $fh;

    my $memory_file = "$output_dir/$pdf_base_name.memory";

    if ( $file_exists_warning && -e $memory_file )
    {
        if ( !prompt_yn("warning: memory file already exists: $memory_file. overwrite? ") )
        {
            exit;
        }
    }

    open( $fh, '>', $memory_file ) or die "Could not open file $memory_file $!";
    my $fh_log;
    open( $fh_log, '>', "$output_dir/$pdf_base_name.pass1.log" ) or die "Could not open file $output_dir/$pdf_base_name.pass1.log $!";

    my $total           = 0;
    my $in_boundary     = 0;
    my $last_found_line = 0;
    my $line            = 0;
    my $page            = 0;
    my $warnings        = 0;

    foreach (@lines)
    {
        ++$line;
        $_ =~ s/[\r\n]//g;

        if (/^\f/)
        {
            ++$page;
        }

        if ( !$in_boundary && /^\s+Boundary address / )
        {
            $in_boundary = 1;
            next;
        }

        if ( $in_boundary && /^\s*(0x(?:[0-9A-F]){4}\s*(?:[0-9A-F]){4}) - 0x(?:[0-9A-F]){4}\s*(?:[0-9A-F]){4}\s+(.*)/i )
        {
            my $addr = $1;
            my $d    = $2;
            $addr =~ s/ //g;
            $addr =~ s/X/x/;
            my @parts = split /  /, $d, 2;
            if ( $#parts == 1 )
            {
                $d = $parts[0];
            }
            $d =~ s/[\-\&\/]/_/g;
            $d =~ s/ _ /_/g;
            $d =~ s/reg.*$//i;
            $d =~ s/\s*$//;
            $d =~ s/\s+/_/g;
            $d =~ s/[\(\)]+//g;
            $d =~ s/^_+//;
            printf $fh "%-20s : %s\n", $d, $addr;
            ++$total;
            $last_found_line = $line;

            if ( $addr eq $lowest_addr )
            {
                last;
            }
        }
        elsif ( $last_found_line != 0 && $line - $last_found_line > 10 )
        {
            print "Bailing\n";
            last;
        }
        elsif ( $in_boundary && /0x[0-9A-F]{3,}/ )
        {
            print $fh_log "Pass1 warning: THe following line was rejected ($pdf_fname, page $page):\n";
            print $fh_log "$_\n";
            print $fh_log "You can correct it in $txt_name, line $line\n";
            print $fh_log "(may be an empty peripheral name in the peripheral column)\n";
            print $fh_log "=================================================================================\n";
            ++$warnings;
        }
    }

    close $fh;
    close $fh_log;

    print "Done. Total peripherals found: $total\n";
    print "Take a look at the generated file $memory_file and make manual changes as needed\n";
    if ($warnings)
    {
        print "$warnings lines were rejected. Check the log file: $output_dir/$log_file and correct in $txt_name as needed\n";
    }

}

sub pass2
{
    my $pdf_base_name = get_basename($pdf_fname);
    my $txt_name      = "$pdf_base_name.txt";
    if ( !-e $txt_name )
    {
        die "Text file doesn't exist: $txt_name. Run pass1.pl first";
    }

    my $memory_file = "$output_dir/$pdf_base_name.memory";

    if ( !-e $memory_file )
    {
        die "Memory file doesn't exist: $memory_file. Run pass1.pl first";
    }

    my $regs = "$output_dir/$pdf_base_name.regs";
    open(my $fh_regs, '>', $regs ) or die "Could not open file '$regs' $!";

    my $regs_404 = "$output_dir/$pdf_base_name.regs.404";
    open(my $fh_regs_404, '>', $regs_404 ) or die "Could not open file '$regs_404' $!";


    # Read and parse memory file
    my $fh;
    my %mem;
    open( $fh, '<', $memory_file ) or die "Could not open file '$memory_file' $!";

    my $cnt = 0;
    while ( my $line = <$fh> )
    {
        ++$cnt;
        my @parts = split /\s+:\s+/, $line;
        if ( $#parts != 1 )
        {
            die "error parsing memory file\n";
        }

        # print "<$parts[0]> -> $parts[1]\n";

        if ( exists $mem{ $parts[0] } && lc( $parts[0] ) ne 'reserved' )
        {
            die "Duplicate key found: $parts[0]. Line $cnt: $line\nCheck the memory table file and edit as needed: $memory_file\n";
        }

        $mem{ $parts[0] } = $parts[1];
    }

    close($fh);

    my $sep = "\r";
    if ( File::Spec->catfile( '', '' ) eq "\\" )
    {
        $sep = "\n|\r";
    }

    open my $fh2, '<', $txt_name or die "Can't open file '$txt_name' $!";
    read $fh2, my $file_content, -s $fh2;
    my @lines = split /$sep/, $file_content;
    close($fh2);

    my $page = 0;
    my $str;
    my $reg_found = 0;
    my $regx_found = 0;
    my $reg_missed = 0;

    for ( my $l = 0 ; $l < $#lines - 1 ; ++$l )
    {
        $str = $lines[$l];
        $str =~ s/[\r\n]//g;

        if ( $str =~ /^\f/ )
        {
            ++$page;
        }

        if ( $str =~ /^\s+Address offset[s]*:/ || $str =~ /^\s+Address:/ )
        {
            # offset from beginning of string is too far, probably a table
            if ( $+[0] > 100 )
            {
                next;
            }


            # Searching for Reset value below: this is optional, may not need it
            my $cnt   = 20;
            my $found = 0;
            for ( my $i = 0 ; $i < $cnt ; ++$i )
            {
                if ( $lines[ $l + $i ] =~ /[Rr]eset [Vv]alue[s]*:/ )
                {
                    $found = 1;
                    last;
                }
            }
            if ( !$found )
            {
                next;
            }

            # Scanning backwards: looking for register name
            $found = 0;
            $cnt   = 20;
            my $reg;
            my $regline = "";
            for ( my $i = 1 ; $i < $cnt ; ++$i )
            {
                # Underscore is sometimes missing!
                 # if ( $lines[ $l - $i ] =~ /(\([A-Z0-9]+[x]?_[A-Z0-9]+[x]?.*\))/ )
                 if ( $lines[ $l - $i ] =~ /(\((?:[A-Z0-9_xi]+){5,}\).*)/ )
                 {
                    $found   = 1;
                    $regline = $lines[ $l - $i ];
                    $reg     = $1;
                    last;
                 }
            }

            if ($found)
            {
                ++$reg_found;
                print $fh_regs "str: $str\n";
                print $fh_regs "regline: $regline, line $l, page: $page\n";
                print $fh_regs "reg: $reg\n";
                if ($reg =~ /(\(.*\))(.+)$/) 
                {
                    $reg = $1; 
                    my $tail = $2;
                    print $fh_regs "tail: $tail\n"; 
                    ++$regx_found;
                    if ($reg !~ /x/) 
                    {
                       print $fh_regs "warning: tail found, but reg name has no 'x'\n";
                    }
                }
                $reg =~ s/[\(\)]//g;
                print $fh_regs "freg: $reg\n";
                print $fh_regs "======================================================\n";

            }
            else
            {
                ++$reg_missed;
                print $fh_regs_404 "line $l, page: $page: NOT FOUND: $str\n";
                for ( my $i = 0 ; $i < 20 ; ++$i )
                {
                   print $fh_regs_404 "$lines[ $l - 10 + $i ]\n";
                }
                print $fh_regs_404 "===============================================================\n";
            }
        }
    }

    print $fh_regs "reg found: $reg_found, regx found: $regx_found\n";
    print $fh_regs_404 "missed: $reg_missed\n";

    close ($fh_regs);
    close ($fh_regs_404);
}

my $step    = -1;
my %options = ();
getopts( "s:", \%options );
if ( $options{s} eq '1' || $options{s} eq '2' )
{
    $step = $options{s};
}

if ( $step == -1 || $#ARGV != 0 )
{
    die "usage: llgen.pl -s [1|2] <datasheet.pdf>";
}

$pdf_fname = $ARGV[0];
if ( !-e $pdf_fname )
{
    die "file doesn't exist: $pdf_fname";
}

$step == 1 ? pass1 : pass2;
