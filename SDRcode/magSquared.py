#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Magsquared
# Generated: Sun Oct 28 00:26:26 2012
##################################################

from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import uhd
from gnuradio.eng_option import eng_option
from gnuradio.gr import firdes
from optparse import OptionParser
import threading
import time
import math
import socket
import sys

class magSquared(gr.top_block):

	def __init__(self):


		gr.top_block.__init__(self, "Magsquared")

		##################################################
		# Variables
		##################################################
		self.variable_function_probe_0 = variable_function_probe_0 = 0
		self.samp_rate = samp_rate = 70e3
		self.gain = gain = 50
		self.center_freq = center_freq = 904e6

		##################################################
		# Blocks
		##################################################
		self.gr_probe_avg_mag_sqrd_x_0 = gr.probe_avg_mag_sqrd_c(0, 1e-3)
		def _variable_function_probe_0_probe():
			if(len(sys.argv) == 3):
				s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
				s.connect((sys.argv[1],int(sys.argv[2])))
			while True:
				val = self.gr_probe_avg_mag_sqrd_x_0.level()
				if (val > 0):
					valdb = 10 * math.log10(val)
					print "Mag Squared (dB): {0}".format(valdb)
					if(len(sys.argv) == 3):
						s.send("Mag Squared (dB): {0}".format(valdb))
				try: self.set_variable_function_probe_0(val)
				except AttributeError, e: pass
				time.sleep(1.0/(100))
		_variable_function_probe_0_thread = threading.Thread(target=_variable_function_probe_0_probe)
		_variable_function_probe_0_thread.daemon = True
		_variable_function_probe_0_thread.start()
		self.uhd_usrp_source_0 = uhd.usrp_source(
			device_addr="",
			stream_args=uhd.stream_args(
				cpu_format="fc32",
				channels=range(1),
			),
		)
		self.uhd_usrp_source_0.set_samp_rate(samp_rate)
		self.uhd_usrp_source_0.set_center_freq(center_freq, 0)
		self.uhd_usrp_source_0.set_gain(gain, 0)
		self.uhd_usrp_source_0.set_antenna("TX/RX", 0)
		self.low_pass_filter_0 = gr.fir_filter_ccf(1, firdes.low_pass(
			1, samp_rate, 5000, 1000, firdes.WIN_HAMMING, 6.76))

		##################################################
		# Connections
		##################################################
		self.connect((self.uhd_usrp_source_0, 0), (self.low_pass_filter_0, 0))
		self.connect((self.low_pass_filter_0, 0), (self.gr_probe_avg_mag_sqrd_x_0, 0))

	def get_variable_function_probe_0(self):
		return self.variable_function_probe_0

	def set_variable_function_probe_0(self, variable_function_probe_0):
		self.variable_function_probe_0 = variable_function_probe_0

	def get_samp_rate(self):
		return self.samp_rate

	def set_samp_rate(self, samp_rate):
		self.samp_rate = samp_rate
		self.uhd_usrp_source_0.set_samp_rate(self.samp_rate)
		self.low_pass_filter_0.set_taps(firdes.low_pass(1, self.samp_rate, 5000, 1000, firdes.WIN_HAMMING, 6.76))

	def get_gain(self):
		return self.gain

	def set_gain(self, gain):
		self.gain = gain
		self.uhd_usrp_source_0.set_gain(self.gain, 0)

	def get_center_freq(self):
		return self.center_freq

	def set_center_freq(self, center_freq):
		self.center_freq = center_freq
		self.uhd_usrp_source_0.set_center_freq(self.center_freq, 0)

if __name__ == '__main__':
	parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
	(options, args) = parser.parse_args()
	tb = magSquared()
	tb.start()
	raw_input('Press Enter to quit: ')
	tb.stop()

