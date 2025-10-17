<?php;
$liczba=10;
echo $liczba;

$liczba=15;
echo $liczba;

$nauczyciele=[
    "Szalkowski"=>"Maciej",
    "Kowalski"=>"Maciej",
    "Kamiński"=>"Maciej",
];

$nauczyciele["Szalkowski"]="Grzegorz";

echo $nauczyciele["Szalkowski"];
echo $nauczyciele["Kamiński"];

$nauczyciele["Switon"]="Dariusz";
echo $nauczyciele["Switon"];