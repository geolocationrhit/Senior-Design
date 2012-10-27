$GPRMC,231429.00,V,,,,,,,261012,,,N*74                                          
$GPVTG,,,,,,,,,N*30                                                             
$GPGGA,231429.00,,,,,0,00,99.99,,,,,,*69                                        
$GPGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*30                                     
$GPGSV,1,1,00*79                                                                
$GPGLL,,,,,231429.00,V,N*45  

Example Readout w/o fix.


Send These commands first:
echo 0 > /sys/kernel/debug/omap_mux/uart1_txd
echo 20 > /sys/kernel/debug/omap_mux/uart1_rxd

minicom -b 9600 -D /dev/ttyO1


