#!/bin/env perl

$file = $ARGV[0];

open FILE,"<$file" or die "Cannot read $file: $!\n";
open TMP,">/tmp/wrapfile" or die "Cannot create tmp file! $!\n";

while($line = <FILE>)
{
    if ($line =~ m/^(\s+\.def)/o)
    {
        $line =~ s/.def/wrapper.def/o;
        chomp $line;
        print TMP "$line;\n";
    }
    else
    {
        print TMP "$line";
    }
}

close FILE;
close TMP;

system("mv /tmp/wrapfile $file");

