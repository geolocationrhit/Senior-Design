#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Spec Analyzer
# Generated: Wed Jan  9 23:32:04 2013
##################################################

#from PyQt4 import Qt
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import uhd
from gnuradio.eng_option import eng_option
from gnuradio.gr import firdes
from optparse import OptionParser
import sys

class spec_analyzer(gr.top_block):

	def __init__(self):
		gr.top_block.__init__(self, "Spec Analyzer")

		##################################################
		# Variables
		##################################################
		self.samp_rate = samp_rate = 125e3

		##################################################
		# Blocks
		##################################################
		self.uhd_usrp_sink_0 = uhd.usrp_sink(
			device_addr="",
			stream_args=uhd.stream_args(
				cpu_format="fc32",
				channels=range(1),
			),
		)
		self.uhd_usrp_sink_0.set_samp_rate(samp_rate)
		self.uhd_usrp_sink_0.set_center_freq(925e6, 0)
		self.uhd_usrp_sink_0.set_gain(0, 0)
		self.uhd_usrp_sink_0.set_antenna("TX/RX", 0)
		self.const_source_x_0 = gr.sig_source_c(0, gr.GR_CONST_WAVE, 0, 0, .02)

		##################################################
		# Connections
		##################################################
		self.connect((self.const_source_x_0, 0), (self.uhd_usrp_sink_0, 0))

	def get_samp_rate(self):
		return self.samp_rate

	def set_samp_rate(self, samp_rate):
		self.samp_rate = samp_rate
		self.uhd_usrp_sink_0.set_samp_rate(self.samp_rate)

if __name__ == '__main__':
	parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
	(options, args) = parser.parse_args()
	tb = spec_analyzer()
	tb.start()
	raw_input('Press ENTER to quit:')
	tb.stop()

