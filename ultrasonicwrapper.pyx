cdef extern void initSensor()
cdef extern void freeSensor()
cdef extern void setTX()
cdef extern void clearTX()
cdef extern int readRX()

class Sensor:
	def __init__(self):
		initSensor()
		print("Initalization of GPIO successful")
		
	def __del__(self):
		freeSensor()
	
	def TX_set(self):
		setTX()
		
	def TX_clear(self):
		clearTX()
		
	def RX_read(self):
		return readRX()
