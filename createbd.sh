# Creating build-enviroment
export BD=$PWD/builddeps
mkdir -p $BD
export PATH=$BD/bin:$PATH
export PKG_CONFIG_PATH=$BD/share/pkgconfig:$BD/lib/pkgconfig:$PKG_CONFIG_PATH

# Clone and build libev from fnchooft
git clone git://github.com/fnchooft/libev 
cd libev
./configure --prefix $BD
make install
cd ..

# Clone and build hiredis from fnchooft
git clone git://github.com/fnchooft/hiredis 
cd hiredis
make install PREFIX=$BD
cd ..

