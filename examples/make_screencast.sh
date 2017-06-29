clear && asciinema rec rec.json -c "LD_LIBRARY_PATH=/nobackup/inst/lib PYTHONPATH=/nobackup/inst/lib/python2.7/site-packages python example.py" 
docker run --rm -v $PWD:/data asciinema/asciicast2gif -s 2 -t solarized-dark rec.json chatty.gif  
