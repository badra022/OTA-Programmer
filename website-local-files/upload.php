<?php
$target_dir = "uploads/";
$target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);
$uploadOk = 1;
$imageFileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));

// Check if file already exists
if (file_exists($target_file)) {
  if (!unlink("uploads/main.txt")) { 
		echo ("the older version cannot be deleted due to an error"); 
		$uploadOk = 0;
	} 
	else { 
		echo ("older version has been deleted"); 
	} 
}

// Check file size
if ($_FILES["fileToUpload"]["size"] > 1000000) {
  echo "Sorry, your file is too large.";
  $uploadOk = 0;
}

// Allow certain file formats
if($imageFileType != "txt") {
  echo "Sorry, only TXT files are allowed.";
  $uploadOk = 0;
}

// Check if $uploadOk is set to 0 by an error
if ($uploadOk == 0) {
  echo "Sorry, your file was not uploaded.";
  file_put_contents('flag.txt', '0');
// if everything is ok, try to upload file
} else {
  if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {
    echo "The file ". htmlspecialchars( basename( $_FILES["fileToUpload"]["name"])). " has been uploaded.";
	file_put_contents("uploads/length.txt",count(file("uploads/main.txt")));
	file_put_contents('flag.txt', '1');
  } else {
    echo "Sorry, there was an error uploading your file.";
	file_put_contents('flag.txt', '0');
  }
}

?>