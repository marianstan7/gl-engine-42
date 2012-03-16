#!/usr/local/bin/php
<?php
// script.php for  in /home/jochau_g//Desktop/db9
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar 16 17:24:04 2012 gael jochaud-du-plessix
// Last update Fri Mar 16 17:26:21 2012 gael jochaud-du-plessix
//

$file = file_get_contents('DB9.obj');
$lines = explode("\n", $file);
$i = 0;
foreach ($lines as $line)
{
  $parts = explode(" ", $line, 2);
  if ($parts[0] == 'usemtl')
    echo 'g ' . $parts[1] . '_' . $i++ . "\n";
  echo $line . "\n";
}