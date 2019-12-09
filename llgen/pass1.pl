# Parse the memory layout table and generate an intermediate memory file 
# with peripheral names and their addresses 
# Those can then be edited manually before the second pass which parse registers and attempt to match 
# the register names with the peripheral names in the generated memory file 


use warnings;
use strict;
use File::Basename;
use File::Spec;

our $output_dir;
our $log_file;
our $memory_file_prefix;

# pdftotext or ghostscript
# Full or relative path if it is on PATH
my $pdftotext_path = "pdftotext.exe";
my $pdfotext_opts = "-simple";

require "./cfg.pl";

# check if memory file already exists
#my $file_exists_warning = 1;

# overwrite silently, manual changes will be lost
my $file_exists_warning = 0;

# lowest peripheral address to expect when parsing memory table
my $lowest_addr = "0x40000000";

if ($#ARGV != 0) 
{
  die "usage: scan <datasheet.pdf>";
}

my $pdf_fname = $ARGV[0];
if (! -e $pdf_fname) 
{
   die "file doesn't exist: $pdf_fname";
}

sub prompt {
  my ($query) = @_; # take a prompt string as argument
  local $| = 1; # activate autoflush to immediately show the prompt
  print $query;
  chomp(my $answer = <STDIN>);
  return $answer;
}

sub prompt_yn {
  my ($query) = @_;
  my $answer;
  do 
  {
    $answer = prompt("$query (y/n): ");
  }
  while ($answer ne 'y' && $answer ne 'n');
  return lc($answer) eq 'y';
}

my $pdf_base_name = get_basename($pdf_fname);
my $txt_name = "$pdf_base_name.txt";
if (! -e $txt_name) 
{
   print "Processing $pdf_fname and generating text file: $txt_name...\n";
   system ("$pdftotext_path $pdfotext_opts \"$pdf_fname\" \"$txt_name\"");
   if ($? != 0) 
   {
      die "Error running pdf converter command, check if $pdftotext_path exists";
   }
   print "Done!\n"
}

mkdir $output_dir unless -d $output_dir;

my $fh;
open($fh, '<', $txt_name) or die "Could not open file '$txt_name' $!";

my $sep = "\r";
if (File::Spec->catfile('', '') eq "\\")
{
  $sep = "\n|\r";
}

read $fh, my $file_content, -s $fh;
my @lines = split /$sep/, $file_content; 
close $fh;

my $memory_file = "$output_dir/$memory_file_prefix.$pdf_base_name";

if ($file_exists_warning && -e $memory_file) 
{
  if (!prompt_yn("warning: memory file already exists: $memory_file. overwrite? ")) 
  {
     exit;
  }
}

open($fh, '>', $memory_file) or die "Could not open file $memory_file $!";
my $fh_log;
open($fh_log, '>', "$output_dir/$log_file") or die "Could not open file $output_dir/$log_file $!";

my $total = 0;
my $in_boundary=0;
my $last_found_line=0;
my $line = 0;
my $page = 0;
my $warnings = 0;

foreach (@lines)
{
    ++$line;
    $_ =~ s/[\r\n]//g;

    if (/^\f/)
    {
      ++$page;
    }

    if (!$in_boundary && /^\s+Boundary address /)
    {
       $in_boundary = 1;
       next;
    } 

    if ($in_boundary && /^\s*(0x(?:[0-9A-F]){4}\s*(?:[0-9A-F]){4}) - 0x(?:[0-9A-F]){4}\s*(?:[0-9A-F]){4}\s+(.*)/i)
    {
       my $addr = $1;
       my $d = $2;
       $addr =~ s/ //g;
       $addr =~ s/X/x/;
       my @parts = split /  /, $d, 2;
       if ($#parts == 1) 
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
       printf $fh "%-20s : %s\n",   $d, $addr;
       ++$total;
       $last_found_line = $line;
       if ($addr eq $lowest_addr) 
       {
         last;
       }
    }
    elsif ($last_found_line != 0 && $line - $last_found_line > 10) 
    {
       print "Bailing\n"; 
       last;
    }
    elsif ($in_boundary && /0x[0-9A-F]{3,}/)
    {
      print $fh_log "Pass1 warning: THe following line was rejected ($pdf_fname, page $page):\n";
      print $fh_log "$_\n";
      print $fh_log "You can correct it in $txt_name, line $line\n"; 
      print $fh_log "(may be an empty peripheral name in the peripheral column";
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