# serve mp4 file
curl -s --output test0.mp4 localhost:8001/toto/video_3min.mp4
diff test0.mp4 ./public/video_3min.mp4
rm test0.mp4

# serve txt file 
curl -s --output test4.txt localhost:8001/toto/txt.txt
diff test4.txt ./public/txt.txt
rm test4.txt

# serve jpeg file 
curl -s --output test1.jpeg localhost:8001/toto/image.jpeg
diff test1.jpeg ./public/image.jpeg
rm test1.jpeg

# serve gif file
curl -s --output test2.gif localhost:8001/toto/gunsNRoses.gif
diff test2.gif ./public/gunsNRoses.gif
rm test2.gif 
