# Shell script which builds OpenSSL and CURL for macOS (universal binary)

cd "$(dirname "$0")"/../ext
root_dir=$(pwd)

# Clone openssl (TODO : Use curl to reduce time)
git clone https://github.com/openssl/openssl
cd openssl
git checkout OpenSSL_1_1_1m
cd ..
cp -r openssl openssl_x86_64
mv openssl openssl_arm64

# Build openssl for x86_64 and arm64
cd ./openssl_x86_64
pwd=$(pwd)
./Configure darwin64-x86_64-cc shared no-idea no-md2 no-mdc2 no-rc5 no-rc4 no-engine --prefix=${pwd}/build
make -j -s
cd ..

# Build openssl for arm64
cd ./openssl_arm64
pwd=$(pwd)
./Configure darwin64-arm64-cc shared no-idea no-md2 no-mdc2 no-rc5 no-rc4 no-engine --prefix=${pwd}/build
make -j -s
cd ..

# Generate universal binaries
mkdir openssl_universal
mkdir openssl_universal/lib
lipo -create ${root_dir}/openssl_x86_64/libcrypto.1.1.dylib ${root_dir}/openssl_arm64/libcrypto.1.1.dylib -output ${root_dir}/openssl_universal/lib/libcrypto.dylib
lipo -create ${root_dir}/openssl_x86_64/libssl.1.1.dylib ${root_dir}/openssl_arm64/libssl.1.1.dylib -output ${root_dir}/openssl_universal/lib/libssl.dylib

install_name_tool -id @rpath/libcrypto.dylib ${root_dir}/openssl_universal/lib/libcrypto.dylib
install_name_tool -id @rpath/libssl.dylib ${root_dir}/openssl_universal/lib/libssl.dylib

echo ${root_dir}/openssl_x86_64/build/lib/libcrypto.1.1.dylib
install_name_tool -change ${root_dir}/openssl_x86_64/build/lib/libcrypto.1.1.dylib @rpath/libcrypto.dylib ${root_dir}/openssl_universal/lib/libssl.dylib
install_name_tool -change ${root_dir}/openssl_arm64/build/lib/libcrypto.1.1.dylib @rpath/libcrypto.dylib ${root_dir}/openssl_universal/lib/libssl.dylib

# Copy headers
cp -r ${root_dir}/openssl_arm64/include openssl_universal/include

# Clone curl (TODO : Use curl to reduce time)
git clone https://github.com/curl/curl;
cd curl;
git checkout curl-7_81_0;

# Build curl
mkdir build;
cd build;
cmake -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" \
-DOPENSSL_CRYPTO_LIBRARY=${root_dir}/openssl_universal/lib/libcrypto.dylib \
-DOPENSSL_SSL_LIBRARY=${root_dir}/openssl_universal/lib/libssl.dylib \
-DOPENSSL_INCLUDE_DIR=${root_dir}/openssl_universal/include  \
-DCURL_USE_LIBSSH2=OFF \
-GNinja ..;
ninja

# Cleanup
rm -rf ${root_dir}/openssl_arm64
rm -rf ${root_dir}/openssl_x86_64


