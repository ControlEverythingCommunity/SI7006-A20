# Distributed with a free-will license.
# Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
# SI7006-A20
# This code is designed to work with the SI7006-A20_I2CS I2C Mini Module available from ControlEverything.com.
# https://www.controleverything.com/content/Humidity?sku=SI7006-A20_I2CS#tabs-0-product_tabset-2

from OmegaExpansion import onionI2C
import time

# Get I2C bus
i2c = onionI2C.OnionI2C()

# SI7006_A20 address, 0x40(64)
#		0xF5(245)	Select Relative Humidity NO HOLD MASTER mode
bytes = [0xF5]
i2c.write(0x40, bytes)

time.sleep(0.5)

# SI7006_A20 address, 0x40(64)
# Read data back, 2 bytes, Humidity MSB first
data = i2c.readBytes(0x40, 0x00, 1)
data1 = i2c.readBytes(0x40, 0x00, 1)

# Convert the data
humidity = (125.0 * (data0 * 256.0 + data1) / 65536.0) - 6.0

# SI7006_A20 address, 0x40(64) 
#		0xF3(243)	Select temperature NO HOLD MASTER mode
bytes = [0xF3]
i2c.write(0x40, bytes)
time.sleep(0.5)

# SI7006_A20 address, 0x40(64)
# Read data back, 2 bytes, Temperature MSB first
data = i2c.readBytes(0x40, 0x00, 1)
data1 = i2c.readBytes(0x40, 0x00, 1)
# Convert the data
cTemp = (175.72 * (data0 * 256.0 + data1) / 65536.0) - 46.85
fTemp = cTemp * 1.8 + 32

# Output data to screen
print "Relative Humidity is : %.2f %%RH" %humidity
print "Temperature in Celsius is : %.2f C" %cTemp
print "Temperature in Fahrenheit is : %.2f F" %fTemp
