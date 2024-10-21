# N2HE-FacialVerification-Demo

## Prerequisites
- [OpenSSL](https://www.openssl.org/)  3.2.1 or later

## Installation
Supported platforms: Linux.  

```
cd build
cmake ..
make
```

## Usage
```
./init       // generate keys
./enc        // encrypt the features
./enc_server // encrypt the model
./eval       // homomorphically inference the encrypted model, using the encrypted features
./dec        // decrypt the result
```
