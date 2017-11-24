#!/bin/bash
sudo sshpass -p "toor" scp -r ../../SpiApp pi@10.100.93.33:/home/pi
sudo sshpass -p "toor" scp -r ../../SpiApp pi@10.100.93.40:/home/pi
#sudo scp -r ../../SpiApp pi@10.100.93.33:/home/pi
#sudo scp -r uart_test_reciever pi@10.100.93.33:/home/pi
#sudo scp -r uart_test_transmitter pi@10.100.93.33:/home/pi
#sudo scp -r ../../SpiApp pi@10.100.93.40:/home/pi
#sudo scp -r uart_test_reciever pi@10.100.93.32:/home/pi
#sudo scp -r uart_test_transmitter pi@10.100.93.32:/home/pi

