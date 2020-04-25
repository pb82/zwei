#ifndef ZWEI_EMBEDDEDASSET_H
#define ZWEI_EMBEDDEDASSET_H

#include <string>
#include <tuple>

#include <BASE64/base64.h>

class EmbeddedAsset {
public:
    EmbeddedAsset(std::string &&encoded) {
        Base64::Decode(encoded, &decoded);
    }

    std::tuple<void *, unsigned int> raw() {
        auto *ptr = (void *) decoded.c_str();
        return {ptr, decoded.size()};
    }

private:
    std::string decoded;
};

#endif
