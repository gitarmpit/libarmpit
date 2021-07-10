use warnings;
use strict;
use File::Basename;
use File::Spec;

if ($#ARGV != 0 )
{
    die "usage: parse.pl fname";
}

my $fname = $ARGV[0];


my $fh;
open( $fh, '<', $fname ) or die "Could not open file '$fname' $!";

my $sep = "\r";
if ( File::Spec->catfile( '', '' ) eq "\\" )
{
    $sep = "\n|\r";
}

read $fh, my $file_content, -s $fh;
my @lines = split /$sep/, $file_content;
close $fh;

my $base = 0;
my $offset = 0;

foreach (@lines)
{
    $_ =~ s/[\r\n]//g;
    if (/^(4[0-9A-F_]+) .*\((.*_.*)\) /i)
    {
       my $addr = "0x" . $1;
       my $reg = $2;
       $addr =~ s/_//; 
       if ($base eq 0) 
       {
          $base = $addr;
       }
       my $off = hex($addr) - hex($base);
       printf "#define %s    (*(volatile uint32_t*)(PORT_BASE + 0x%X)) // 0x%X\n", $reg, $off, hex($base) + $off;
    }

}