# Pass 2: Parse the registers and generated code 

use warnings;
use strict;
use File::Spec;

my $pdf_fname = $ARGV[0];


# my $sep = File::Spec->catfile('', '');
#print "$sep\n";
#require ".${sep}pass1.pl";

our $output_dir;
our $log_file;
our $memory_file_prefix;
our $address_offset;

require "./cfg.pl";

if (! -e $pdf_fname) 
{
   die "file doesn't exist: $pdf_fname";
}

my $pdf_base_name = get_basename($pdf_fname);
my $txt_name = "$pdf_base_name.txt";
if (! -e $txt_name) 
{
  die "Text file doesn't exist: $txt_name. Run pass1.pl first";  
}

my $memory_file = "$output_dir\\$memory_file_prefix.$pdf_base_name";

if (! -e $memory_file) 
{
  die "Memory file doesn't exist: $memory_file. Run pass1.pl first";  
}


# Read and parse memory file 
my $fh;
my %mem;
open($fh, '<', $memory_file) or die "Could not open file '$memory_file' $!";

my $cnt = 0;
while (my $line = <$fh>)
{
  ++$cnt;
  my @parts = split /\s+:\s+/, $line; 
  if ($#parts != 1) 
  {
     die "error parsing memory file\n";
  }
  # print "<$parts[0]> -> $parts[1]\n";

  if (exists $mem{$parts[0]} && lc($parts[0]) ne 'reserved' ) 
  {
     die "Duplicate key found: $parts[0]. Line $cnt: $line\nCheck the memory table file and edit as needed: $memory_file\n"; 
  }

  $mem{$parts[0]} = $parts[1];
}

close ($fh);

my $sep = "\r";
if (File::Spec->catfile('', '') eq "\\")
{
  $sep = "\n|\r";
}

open my $fh2, '<', $txt_name or die "Can't open file $txt_name $!";
read $fh2, my $file_content, -s $fh2;
my @lines = split /$sep/, $file_content; 
close($fh2);


# Check for inconsistencies: "Address offset:" is sometimes just "Address: "
# So need to also check for "Reset values:", hoping that both are consistent 
# If not, need to check the datasheet and find a better search pattern 

my $total_addr = 0;
my $page = 0;
my $str;
my $missed_resets = 0;

for (my $l = 0; $l < $#lines-1; ++$l)  
{
  $str = $lines[$l];
  $str =~ s/[\r\n]//g;

  if ($str =~ /^\f/)
  {
    ++$page;
  }
  
  if ($str =~ /^\s+Address offset[s]*:/ || $str =~ /^\s+Address:/) 
  {
       # offset from beginning of string is too far, probably a table
       if ($+[0] > 100) 
       {
         next;
       }

     # print "$_\n";
       ++$total_addr;
       my $cnt = 20; 
       my $found = 0;
       for (my $i = 0; $i < $cnt; ++$i) 
       {
         if ($lines[$l+$i] =~ /[Rr]eset [Vv]alue[s]*:/)
         {
            $found = 1; 
            last;
         }
       }
       if (! $found) 
       {
          ++ $missed_resets;
          print "Reset not found near Address offset, line: " . ($l+1) . ", page: $page\n";
          print "$str\n";
          for (my $i = 0; $i < 20; ++$i) 
          {
             my $mark = ($i != 10) ? "" : " <= "; 
             print "$lines[$l - 10 + $i] $mark";
          }    
  
          print "\n=========================================================================\n";
       }

  }
}

print "total_addr: $total_addr\n";
print "missed reset addrs: $missed_resets\n";


print "total pages: $page\n";
