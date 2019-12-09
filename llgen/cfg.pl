# Common variables 

use warnings;
use strict;

our $output_dir = "out";
our $log_file = "log.txt";
our $memory_file_prefix = "memory";

# patterns 
# Example: Address offset: 0x1FD0  (on a line by itself)
# our $address_offset = "Address offset:\\s+(0x[0-9A-F]{4})\\s*\$"
# more relaxed in case there are typos
our $address_offset = "Address offset:\\s+(\\S+)\\s*\$";


#########################################################

sub get_basename 
{
    my @parts = split /[\/\\]/, $_[0];
    my $base = $parts[$#parts];
    $base =~ s/^(.*)\.pdf$/$1/;
    if ($base eq "") 
    {
       die "error parsing filename";
    }
    return $base;
}
