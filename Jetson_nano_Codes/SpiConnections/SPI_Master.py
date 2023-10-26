import spidev
import time

spi = spidev.SpiDev()
spi.open(0, 0) 

# Example values to be sent
distance = 1.5
velocity = 2.5
direction = ord('N')

# Sending data to ESP32
spi.xfer([int(distance * 100), int(velocity * 100), direction])

# Receiving data from ESP32
received_data = spi.readbytes(12)  # 3 integers * 4 bytes each

# Converting received data back to integer format
acceleration = [int(received_data[i]) for i in range(0, 3)]
received_velocity = [int(received_data[i + 3]) for i in range(0, 3)]
position = [int(received_data[i + 6]) for i in range(0, 3)]
angles = [int(received_data[i + 9]) for i in range(0, 3)]

# Processing the received data

# Add your processing code here

time.sleep(0.05)  # Adjust the delay as needed
