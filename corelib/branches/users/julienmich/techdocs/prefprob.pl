
$p[0] = 0.1;
$p[1] = 0.3;
$p[2] = 0.6;

$max = $p[2];

$n[0] = 0;
$n[1] = 0;
$n[2] = 0;

for ($i=0; $i<10000; $i++)
{
    $j = 0;

    $found = 0;

    while(not $found)
    {
        $j = int(rand(3.0));

        if (rand($max) <= $p[$j])
        {
            $found = 1;
        }
    }
    
    $n[$j] = $n[$j] + 1;
}

print "$n[0], $n[1], $n[2]\n";

