
while ($line = <STDIN>)
{
    chomp $line;

    @parts = split("::",$line);
    
    $enum = $parts[-1];
    
    print "        .value( \"$enum\", $parts[0]::$enum )\n"
}
