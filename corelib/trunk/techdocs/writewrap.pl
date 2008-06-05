
$types{"int"} = "toInt";
$types{"long"} = "toLong";
$types{"qlonglong"} = "toLongLong";
$types{"short"} = "toShort";
$types{"uint"} = "toUInt";
$types{"ulong"} = "toULong";
$types{"qulonglong"} = "toULongLong";
$types{"ushort"} = "toUShort";

for $type (keys %types)
{
    print "$type wrap_$types{$type}2(const QString &string, bool *ok, ".
          "int base)\n".
          "{\n".
          "    return string.$types{$type}(ok,base);\n".
          "}\n\n";
          
    print "$type wrap_$types{$type}1(const QString &string, bool *ok)\n".
          "{\n".
          "    return string.$types{$type}(ok);\n".
          "}\n\n";
          
    print "$type wrap_$types{$type}0(const QString &string)\n".
          "{\n".
          "    return string.$types{$type}();\n".
          "}\n\n";
          
}
