# JR zbar install
# can use in shell or other hooks for qr and other decodes

#!/bin/sh
sudo apt-get install zbar-tools

#zbarcam -nodisplay so we don't have a window up
#zbarcam for just normal display
#should dump in standard out unless we use piping

#for just doing an img processing
#zbarimg <imgname.extension> tesed with jpg, ping


#can look into hooks in c++ code once I better understand our needs
#time est 30 min


