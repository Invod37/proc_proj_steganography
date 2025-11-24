#include <gtest/gtest.h>
#include <cstdlib>
#include <cstring>
#include "../src/steg_utils.h"
#include "../src/bmp_utils.h"


class StegTest : public ::testing::Test {
protected:
    BmpImage img;

    void SetUp() override {
        memset(&img, 0, sizeof(img));
    }

    void TearDown() override {
        if (img.data) {
            free(img.data);
        }
    }

    
    void create_mock_image(int width, int height) {
        img.width = width;
        img.height = height;
        
        
        int row_bytes = width * 3;
        int padding = (4 - (row_bytes % 4)) % 4;
        img.stride = row_bytes + padding;

        size_t data_size = img.stride * height;
        img.data = (unsigned char*)calloc(1, data_size);
        
        memset(img.data, 0xFF, data_size);
    }
};



TEST_F(StegTest, RoundTripSuccess) {
    create_mock_image(10, 10);
    
    const char* secret = "Hello World";
    size_t msg_len = strlen(secret);

    
    int res = steg_encode_message(&img, (const unsigned char*)secret, msg_len);
    ASSERT_EQ(res, 0) << "Encode returned error code";

    
    unsigned char* decoded_msg = nullptr;
    size_t decoded_len = 0;
    
    res = steg_decode_message(&img, &decoded_msg, &decoded_len);
    ASSERT_EQ(res, 0) << "Decode returned error code";
    
    
    ASSERT_EQ(decoded_len, msg_len);
    ASSERT_STREQ((char*)decoded_msg, secret);

    free(decoded_msg);
}


TEST_F(StegTest, EncodeTooBigMessage) {
    create_mock_image(2, 2);
    
    const char* secret = "BigData"; 
    int res = steg_encode_message(&img, (const unsigned char*)secret, strlen(secret));
    
    ASSERT_EQ(res, -2);
}


TEST_F(StegTest, EncodeEmptyMessage) {
    create_mock_image(10, 10);
    
    const char* secret = "";
    int res = steg_encode_message(&img, (const unsigned char*)secret, 0);
    
    ASSERT_EQ(res, 0);

    unsigned char* decoded_msg = nullptr;
    size_t decoded_len = 0;
    steg_decode_message(&img, &decoded_msg, &decoded_len);

    ASSERT_EQ(decoded_len, 0);
    
    if (decoded_msg) free(decoded_msg);
}


TEST_F(StegTest, BitsAreActuallyChanged) {
    create_mock_image(5, 5);
    
    unsigned char original_byte = img.data[0]; 
    
    steg_encode_message(&img, (const unsigned char*)"", 0);
    
    unsigned char modified_byte = img.data[0];
    
    ASSERT_NE(original_byte, modified_byte);
    ASSERT_EQ(modified_byte & 1, 0);
}