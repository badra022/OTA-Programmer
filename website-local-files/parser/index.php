<?php
if(file_exists ( "../uploads/main.txt") )
{
    $numbers=30;
    $lines = file("../uploads/main.txt");

    if(isset($_GET["Block"]))
    {
        //echo "\n";
        $b=(int)$_GET["Block"];
        for($i = $numbers*($b);$i<count($lines)&&$i<$numbers*($b+1);$i++)
        {
        echo $lines[$i];
		if($i>= count($lines)-1)
			{
				file_put_contents('../flag.txt', '0');
			}	
		}
        if((int)(count($lines)/$numbers)==$b)
        {
            //unlink("uploads/main.txt");
        }
    }

    else
    {
	    echo (int)(count($lines)/$numbers)+1;
    }
}
// GET http://bfota.freevar.com?Block=4
else
    echo 0;
?>