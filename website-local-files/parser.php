<?php


$array = file("uploads/main.txt");
foreach($array as $line)
       {
		   while(file_get_contents('flag.txt') == '1'){}
		   file_put_contents("line.txt", $line);
		   file_put_contents('flag.txt', '1');
           echo $line;
       }
		   file_put_contents('flag.txt', '0');
		   
?>