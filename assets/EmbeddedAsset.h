#ifndef ZWEI_EMBEDDEDASSET_H
#define ZWEI_EMBEDDEDASSET_H

#include <string>

#include <BASE64/base64.h>

class EmbeddedAsset {
public:
    EmbeddedAsset(std::string &&encoded) {
        Base64::Decode(encoded, &decoded);
    }

    void *operator()() {
        return (void *) decoded.c_str();
    }

private:
    std::string decoded;
};

#endif
