use warnings;
use strict;
use File::Spec;

my $sep = "\r";
if (File::Spec->catfile('', '') eq "\\")
{
  $sep = "\n|\r";
}

open my $fh2, '<', "H7.txt" or die "Can't open file $!";
#binmode ($fh2);

read $fh2, my $file_content, -s $fh2;
my @lines = split /$sep/, $file_content; 

my $page = 0;

for (my $l = 0; $l < $#lines-1; ++$l)  
{
  my $str = $lines[$l];
  $str =~ s/[\r\n]//g;

  if ($str =~ /^\f/)
  {
    ++$page;
  }
}

print "total lines: $#lines\n";
print "total pages: $page\n";