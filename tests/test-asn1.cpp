#include "unittestframework.h"

#include "formatter.h"
#include "asn1parser.h"
#include "asn1parser.h"
#include <algorithm>

/*
 * alternative for:
 *  CHECK( equalrange(y.datarange, std::vector<uint8_t>{ 0x39,0x36,0x31,0x30,0x32,0x33,0x32,0x30,0x35,0x36,0x30,0x33,0x5a}) == true );
 */
#ifdef USE_CATCH
// note: matchers will probably be added to doctest in v2.4
template<typename RANGE>
class EqualsVector : public Catch::MatcherBase<RANGE> {
    std::vector<uint8_t> rhs;
public:
    /*
     * CHECK_THAT( y.datarange, EqualsVector<decltype(y.datarange)>(0x39,0x36,0x31,0x30,0x32,0x33,0x32,0x30,0x35,0x36,0x30,0x33,0x5a) );
     */
    template<typename...ARGS>
    EqualsVector(ARGS...args) : rhs{uint8_t(args)...} { }

    /*
     * CHECK_THAT( y.datarange, (EqualsVector<decltype(y.datarange)>{0x39,0x36,0x31,0x30,0x32,0x33,0x32,0x30,0x35,0x36,0x30,0x33,0x5a}) );
     */
//  EqualsVector(std::initializer_list<uint8_t> init) : rhs{init} { }

    virtual bool match( const RANGE& lhs ) const override {
        return equalrange(lhs, rhs);
    }
    virtual std::string describe() const override {
        std::ostringstream ss;
        ss << "{" << rhs << "}";
        return ss.str();
    }    
};

TEST_CASE("asn1parser") {
    auto data = std::vector<uint8_t> { 
0x30,0x82,0x01,0x92,0x30,0x82,0x01,0x3c,0xa0,0x03,0x02,0x01,0x02,0x02,0x10,0x38,0xfe,0x09,0xc9,0xa0,0x00,0xed,0x95,0x11,0xd0,0x2d,0x17,0xd7,0xb4,0x08,0xa0,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x04,0x05,0x00,0x30,0x16,0x31,0x14,0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,0x0b,0x52,0x6f,0x6f,0x74,0x20,0x41,0x67,0x65,0x6e,0x63,0x79,0x30,0x1e,0x17,0x0d,0x39,0x36,0x31,0x30,0x32,0x33,0x32,0x30,0x35,0x36,0x30,0x33,0x5a,0x17,0x0d,0x33,0x39,0x31,0x32,0x33,0x31,0x32,0x33,0x35,0x39,0x35,0x39,0x5a,0x30,0x16,0x31,0x14,0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,0x0b,0x54,0x65,0x73,0x74,0x43,0x6f,0x6d,0x70,0x61,0x6e,0x79,0x30,0x5b,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x01,0x05,0x00,0x03,0x4a,0x00,0x30,0x47,0x02,0x40,0x6b,0x7f,0x91,0x58,0xbd,0x1a,0xf9,0x86,0x37,0x51,0xf1,0x85,0x07,0xf4,0x3a,0x94,0x28,0x17,0xac,0xf8,0x9e,0x21,0xf2,0x0b,0x2f,0x80,0xce,0xaf,0x05,0x60,0x10,0x27,0xdd,0x23,0xfe,0x39,0xc5,0xf9,0x81,0x19,0x2b,0x83,0xb1,0xc4,0x01,0x72,0x9a,0x3c,0x4a,0xea,0xb5,0x09,0x75,0xfa,0x6a,0xa1,0xf8,0xd5,0x43,0x59,0x16,0x5f,0x6e,0x7f,0x02,0x03,0x01,0x00,0x01,0xa3,0x67,0x30,0x65,0x30,0x1a,0x06,0x03,0x55,0x04,0x03,0x04,0x13,0x13,0x11,0x4d,0x69,0x63,0x72,0x6f,0x73,0x6f,0x66,0x74,0x20,0x54,0x72,0x69,0x64,0x65,0x6e,0x74,0x30,0x47,0x06,0x03,0x55,0x1d,0x01,0x04,0x40,0x30,0x3e,0x80,0x10,0x12,0xe4,0x09,0x2d,0x06,0x1d,0x1d,0x4f,0x00,0x8d,0x61,0x21,0xdc,0x16,0x64,0x63,0xa1,0x18,0x30,0x16,0x31,0x14,0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,0x0b,0x52,0x6f,0x6f,0x74,0x20,0x41,0x67,0x65,0x6e,0x63,0x79,0x82,0x10,0x06,0x37,0x6c,0x00,0xaa,0x00,0x64,0x8a,0x11,0xcf,0xb8,0xd4,0xaa,0x5c,0x35,0xf4,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x04,0x05,0x00,0x03,0x41,0x00,0x7a,0x4b,0xfa,0xd7,0x6c,0x8b,0xbf,0x1d,0xa7,0xc5,0xcc,0x64,0xe7,0x9f,0x7d,0x9f,0x76,0x89,0x46,0xa9,0x0b,0xc2,0xf6,0xc6,0x88,0x60,0xc4,0x84,0xe0,0x8f,0xd0,0x4e,0x06,0xdb,0xf4,0xf7,0xde,0x0e,0x30,0x49,0x3a,0xdc,0xdd,0x23,0x03,0x1a,0x3f,0x9f,0xef,0xb8,0x4c,0xea,0x90,0x0c,0x6e,0xf0,0xe8,0x4c,0x20,0x4f,0x37,0xbc,0x1d,0x34
    };

    auto x = asn1tlv(makerange(data));

    using equalsvector = EqualsVector<decltype(x.datarange)>;

    CHECK( x.constructed == true );
    CHECK( x.cls == 0 );
    CHECK( x.tagvalue == 0x10 );
    CHECK( x.length == 0x192 );

    CHECK_THAT( x.tagrange, equalsvector(0x30));
    CHECK_THAT( x.lenrange, equalsvector(0x82, 0x01,0x92));
    CHECK( x.datarange == makerange(data.begin()+4, data.end()));

    // the start of the validity period
    CHECK( traverse(x, { 0, 3, 0 }).length == 13 );
    CHECK_THAT( traverse(x, { 0, 3, 0 }).datarange, equalsvector(0x39,0x36,0x31,0x30,0x32,0x33,0x32,0x30,0x35,0x36,0x30,0x33,0x5a) );

    // the entire signed cert
    CHECK( traverse(x, {  }).datarange == makerange(data.begin()+4, data.end()));   // cert+alg+sig body

    // just the cert
    CHECK( traverse(x, { 0 }).datarange== makerange(data.begin()+8, data.begin()+8+316));

    // the version
    CHECK_THAT( traverse(x, { 0, -1 }).datarange, equalsvector(0x02, 0x01, 0x02));  // cert.version

    // the serial
    CHECK_THAT( traverse(x, { 0, 0 }).datarange,    equalsvector(0x38,0xfe,0x09,0xc9,0xa0,0x00,0xed,0x95,0x11,0xd0,0x2d,0x17,0xd7,0xb4,0x08,0xa0)                                                                                                                                                                                                                                                                                                                                                                                                                                         );    // cert.serial
    // the algo
    CHECK_THAT( traverse(x, { 0, 1 }).datarange,    equalsvector(0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x04,0x05,0x00)                                                                                                                                                                                                                                                                                                                                                                                                                                                        );    // cert.algorithm
    CHECK(stringformat("%s", traverse(x, { 0, 1 }).datarange) == "06 09 2a 86 48 86 f7 0d 01 01 04 05 00");

    // the validity period start
    CHECK_THAT( traverse(x, { 0, 3, 0 }).datarange, equalsvector(0x39,0x36,0x31,0x30,0x32,0x33,0x32,0x30,0x35,0x36,0x30,0x33,0x5a)                                                                                                                                                                                                                                                                                                                                                                                                                                                        );  // first timestamp
    // the subject
    CHECK_THAT( traverse(x, { 0, 4, 0 }).datarange, equalsvector(0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,0x0b,0x54,0x65,0x73,0x74,0x43,0x6f,0x6d,0x70,0x61,0x6e,0x79)                                                                                                                                                                                                                                                                                                                                                                                                                     );  // subject
    // the extensions
    CHECK_THAT( traverse(x, { 0, -4, 0 }).datarange,equalsvector(0x30,0x1a,0x06,0x03,0x55,0x04,0x03,0x04,0x13,0x13,0x11,0x4d,0x69,0x63,0x72,0x6f,0x73,0x6f,0x66,0x74,0x20,0x54,0x72,0x69,0x64,0x65,0x6e,0x74,0x30,0x47,0x06,0x03,0x55,0x1d,0x01,0x04,0x40,0x30,0x3e,0x80,0x10,0x12,0xe4,0x09,0x2d,0x06,0x1d,0x1d,0x4f,0x00,0x8d,0x61,0x21,0xdc,0x16,0x64,0x63,0xa1,0x18,0x30,0x16,0x31,0x14,0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,0x0b,0x52,0x6f,0x6f,0x74,0x20,0x41,0x67,0x65,0x6e,0x63,0x79,0x82,0x10,0x06,0x37,0x6c,0x00,0xaa,0x00,0x64,0x8a,0x11,0xcf,0xb8,0xd4,0xaa,0x5c,0x35,0xf4));    // cert.extensions
    // the signature algorithm
    CHECK_THAT( traverse(x, { 1 }).datarange,       equalsvector(0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x04,0x05,0x00)                                                                                                                                                                                                                                                                                                                                                                                                                                                        );         // sig.algorithm

}
#endif

TEST_CASE("asn1iter1") {
    auto data = std::vector<uint8_t> { 
0x30,0x82,0x01,0x92,0x30,0x82,0x01,0x3c,0xa0,0x03,0x02,0x01,0x02,0x02,0x10,0x38,0xfe,0x09,0xc9,0xa0,0x00,0xed,0x95,0x11,0xd0,0x2d,0x17,0xd7,0xb4,0x08,0xa0,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x04,0x05,0x00,0x30,0x16,0x31,0x14,0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,0x0b,0x52,0x6f,0x6f,0x74,0x20,0x41,0x67,0x65,0x6e,0x63,0x79,0x30,0x1e,0x17,0x0d,0x39,0x36,0x31,0x30,0x32,0x33,0x32,0x30,0x35,0x36,0x30,0x33,0x5a,0x17,0x0d,0x33,0x39,0x31,0x32,0x33,0x31,0x32,0x33,0x35,0x39,0x35,0x39,0x5a,0x30,0x16,0x31,0x14,0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,0x0b,0x54,0x65,0x73,0x74,0x43,0x6f,0x6d,0x70,0x61,0x6e,0x79,0x30,0x5b,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x01,0x05,0x00,0x03,0x4a,0x00,0x30,0x47,0x02,0x40,0x6b,0x7f,0x91,0x58,0xbd,0x1a,0xf9,0x86,0x37,0x51,0xf1,0x85,0x07,0xf4,0x3a,0x94,0x28,0x17,0xac,0xf8,0x9e,0x21,0xf2,0x0b,0x2f,0x80,0xce,0xaf,0x05,0x60,0x10,0x27,0xdd,0x23,0xfe,0x39,0xc5,0xf9,0x81,0x19,0x2b,0x83,0xb1,0xc4,0x01,0x72,0x9a,0x3c,0x4a,0xea,0xb5,0x09,0x75,0xfa,0x6a,0xa1,0xf8,0xd5,0x43,0x59,0x16,0x5f,0x6e,0x7f,0x02,0x03,0x01,0x00,0x01,0xa3,0x67,0x30,0x65,0x30,0x1a,0x06,0x03,0x55,0x04,0x03,0x04,0x13,0x13,0x11,0x4d,0x69,0x63,0x72,0x6f,0x73,0x6f,0x66,0x74,0x20,0x54,0x72,0x69,0x64,0x65,0x6e,0x74,0x30,0x47,0x06,0x03,0x55,0x1d,0x01,0x04,0x40,0x30,0x3e,0x80,0x10,0x12,0xe4,0x09,0x2d,0x06,0x1d,0x1d,0x4f,0x00,0x8d,0x61,0x21,0xdc,0x16,0x64,0x63,0xa1,0x18,0x30,0x16,0x31,0x14,0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,0x0b,0x52,0x6f,0x6f,0x74,0x20,0x41,0x67,0x65,0x6e,0x63,0x79,0x82,0x10,0x06,0x37,0x6c,0x00,0xaa,0x00,0x64,0x8a,0x11,0xcf,0xb8,0xd4,0xaa,0x5c,0x35,0xf4,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x04,0x05,0x00,0x03,0x41,0x00,0x7a,0x4b,0xfa,0xd7,0x6c,0x8b,0xbf,0x1d,0xa7,0xc5,0xcc,0x64,0xe7,0x9f,0x7d,0x9f,0x76,0x89,0x46,0xa9,0x0b,0xc2,0xf6,0xc6,0x88,0x60,0xc4,0x84,0xe0,0x8f,0xd0,0x4e,0x06,0xdb,0xf4,0xf7,0xde,0x0e,0x30,0x49,0x3a,0xdc,0xdd,0x23,0x03,0x1a,0x3f,0x9f,0xef,0xb8,0x4c,0xea,0x90,0x0c,0x6e,0xf0,0xe8,0x4c,0x20,0x4f,0x37,0xbc,0x1d,0x34
    };

    auto x = asn1tlv(makerange(data));

    int count = 0;
    for (auto tlv : enumtlvs(x.datarange)) {
        switch(count)
        {
            case 0:
                CHECK( tlv.tagvalue == 16 );
                CHECK( tlv.length== 316 );
                break;
            case 1:
                CHECK( tlv.tagvalue == 16 );
                CHECK( tlv.length== 13 );
                break;
            case 2:
                CHECK( tlv.tagvalue == 3 );
                CHECK( tlv.length== 65 );
                break;
        }
        count++;
    }
    CHECK( count == 3 );
}
#if 0
TEST_CASE("asn1iter1r") {
    auto data = std::vector<uint8_t> { 
0x30,0x82,0x01,0x92,0x30,0x82,0x01,0x3c,0xa0,0x03,0x02,0x01,0x02,0x02,0x10,0x38,0xfe,0x09,0xc9,0xa0,0x00,0xed,0x95,0x11,0xd0,0x2d,0x17,0xd7,0xb4,0x08,0xa0,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x04,0x05,0x00,0x30,0x16,0x31,0x14,0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,0x0b,0x52,0x6f,0x6f,0x74,0x20,0x41,0x67,0x65,0x6e,0x63,0x79,0x30,0x1e,0x17,0x0d,0x39,0x36,0x31,0x30,0x32,0x33,0x32,0x30,0x35,0x36,0x30,0x33,0x5a,0x17,0x0d,0x33,0x39,0x31,0x32,0x33,0x31,0x32,0x33,0x35,0x39,0x35,0x39,0x5a,0x30,0x16,0x31,0x14,0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,0x0b,0x54,0x65,0x73,0x74,0x43,0x6f,0x6d,0x70,0x61,0x6e,0x79,0x30,0x5b,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x01,0x05,0x00,0x03,0x4a,0x00,0x30,0x47,0x02,0x40,0x6b,0x7f,0x91,0x58,0xbd,0x1a,0xf9,0x86,0x37,0x51,0xf1,0x85,0x07,0xf4,0x3a,0x94,0x28,0x17,0xac,0xf8,0x9e,0x21,0xf2,0x0b,0x2f,0x80,0xce,0xaf,0x05,0x60,0x10,0x27,0xdd,0x23,0xfe,0x39,0xc5,0xf9,0x81,0x19,0x2b,0x83,0xb1,0xc4,0x01,0x72,0x9a,0x3c,0x4a,0xea,0xb5,0x09,0x75,0xfa,0x6a,0xa1,0xf8,0xd5,0x43,0x59,0x16,0x5f,0x6e,0x7f,0x02,0x03,0x01,0x00,0x01,0xa3,0x67,0x30,0x65,0x30,0x1a,0x06,0x03,0x55,0x04,0x03,0x04,0x13,0x13,0x11,0x4d,0x69,0x63,0x72,0x6f,0x73,0x6f,0x66,0x74,0x20,0x54,0x72,0x69,0x64,0x65,0x6e,0x74,0x30,0x47,0x06,0x03,0x55,0x1d,0x01,0x04,0x40,0x30,0x3e,0x80,0x10,0x12,0xe4,0x09,0x2d,0x06,0x1d,0x1d,0x4f,0x00,0x8d,0x61,0x21,0xdc,0x16,0x64,0x63,0xa1,0x18,0x30,0x16,0x31,0x14,0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,0x0b,0x52,0x6f,0x6f,0x74,0x20,0x41,0x67,0x65,0x6e,0x63,0x79,0x82,0x10,0x06,0x37,0x6c,0x00,0xaa,0x00,0x64,0x8a,0x11,0xcf,0xb8,0xd4,0xaa,0x5c,0x35,0xf4,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x04,0x05,0x00,0x03,0x41,0x00,0x7a,0x4b,0xfa,0xd7,0x6c,0x8b,0xbf,0x1d,0xa7,0xc5,0xcc,0x64,0xe7,0x9f,0x7d,0x9f,0x76,0x89,0x46,0xa9,0x0b,0xc2,0xf6,0xc6,0x88,0x60,0xc4,0x84,0xe0,0x8f,0xd0,0x4e,0x06,0xdb,0xf4,0xf7,0xde,0x0e,0x30,0x49,0x3a,0xdc,0xdd,0x23,0x03,0x1a,0x3f,0x9f,0xef,0xb8,0x4c,0xea,0x90,0x0c,0x6e,0xf0,0xe8,0x4c,0x20,0x4f,0x37,0xbc,0x1d,0x34
    };

    auto x = asn1tlv(makerange(data));

    int count = 0;
    for (auto & tlv : enumtlvs(x.datarange)) {
        switch(count)
        {
            case 0:
                CHECK( tlv.tagvalue == 16 );
                CHECK( tlv.length== 316 );
                break;
            case 1:
                CHECK( tlv.tagvalue == 16 );
                CHECK( tlv.length== 13 );
                break;
            case 2:
                CHECK( tlv.tagvalue == 3 );
                CHECK( tlv.length== 65 );
                break;
        }
        count++;
    }
    CHECK( count == 3 );
}
#endif


TEST_CASE("asn1iter2") {
    auto data = std::vector<uint8_t> { 
0x30,0x82,0x01,0x92,0x30,0x82,0x01,0x3c,0xa0,0x03,0x02,0x01,0x02,0x02,0x10,0x38,0xfe,0x09,0xc9,0xa0,0x00,0xed,0x95,0x11,0xd0,0x2d,0x17,0xd7,0xb4,0x08,0xa0,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x04,0x05,0x00,0x30,0x16,0x31,0x14,0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,0x0b,0x52,0x6f,0x6f,0x74,0x20,0x41,0x67,0x65,0x6e,0x63,0x79,0x30,0x1e,0x17,0x0d,0x39,0x36,0x31,0x30,0x32,0x33,0x32,0x30,0x35,0x36,0x30,0x33,0x5a,0x17,0x0d,0x33,0x39,0x31,0x32,0x33,0x31,0x32,0x33,0x35,0x39,0x35,0x39,0x5a,0x30,0x16,0x31,0x14,0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,0x0b,0x54,0x65,0x73,0x74,0x43,0x6f,0x6d,0x70,0x61,0x6e,0x79,0x30,0x5b,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x01,0x05,0x00,0x03,0x4a,0x00,0x30,0x47,0x02,0x40,0x6b,0x7f,0x91,0x58,0xbd,0x1a,0xf9,0x86,0x37,0x51,0xf1,0x85,0x07,0xf4,0x3a,0x94,0x28,0x17,0xac,0xf8,0x9e,0x21,0xf2,0x0b,0x2f,0x80,0xce,0xaf,0x05,0x60,0x10,0x27,0xdd,0x23,0xfe,0x39,0xc5,0xf9,0x81,0x19,0x2b,0x83,0xb1,0xc4,0x01,0x72,0x9a,0x3c,0x4a,0xea,0xb5,0x09,0x75,0xfa,0x6a,0xa1,0xf8,0xd5,0x43,0x59,0x16,0x5f,0x6e,0x7f,0x02,0x03,0x01,0x00,0x01,0xa3,0x67,0x30,0x65,0x30,0x1a,0x06,0x03,0x55,0x04,0x03,0x04,0x13,0x13,0x11,0x4d,0x69,0x63,0x72,0x6f,0x73,0x6f,0x66,0x74,0x20,0x54,0x72,0x69,0x64,0x65,0x6e,0x74,0x30,0x47,0x06,0x03,0x55,0x1d,0x01,0x04,0x40,0x30,0x3e,0x80,0x10,0x12,0xe4,0x09,0x2d,0x06,0x1d,0x1d,0x4f,0x00,0x8d,0x61,0x21,0xdc,0x16,0x64,0x63,0xa1,0x18,0x30,0x16,0x31,0x14,0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,0x0b,0x52,0x6f,0x6f,0x74,0x20,0x41,0x67,0x65,0x6e,0x63,0x79,0x82,0x10,0x06,0x37,0x6c,0x00,0xaa,0x00,0x64,0x8a,0x11,0xcf,0xb8,0xd4,0xaa,0x5c,0x35,0xf4,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x04,0x05,0x00,0x03,0x41,0x00,0x7a,0x4b,0xfa,0xd7,0x6c,0x8b,0xbf,0x1d,0xa7,0xc5,0xcc,0x64,0xe7,0x9f,0x7d,0x9f,0x76,0x89,0x46,0xa9,0x0b,0xc2,0xf6,0xc6,0x88,0x60,0xc4,0x84,0xe0,0x8f,0xd0,0x4e,0x06,0xdb,0xf4,0xf7,0xde,0x0e,0x30,0x49,0x3a,0xdc,0xdd,0x23,0x03,0x1a,0x3f,0x9f,0xef,0xb8,0x4c,0xea,0x90,0x0c,0x6e,0xf0,0xe8,0x4c,0x20,0x4f,0x37,0xbc,0x1d,0x34
    };

    auto x = asn1tlv(makerange(data));



    auto en = enumtlvs(x.datarange);
    auto p = en.begin();
    auto last = en.end();

    auto a = *p++;
    auto b = *p++;
    auto c = *p++;

    CHECK( p == last );

    CHECK( a.tagvalue == 16 );
    CHECK( a.length== 316 );

    CHECK( b.tagvalue == 16 );
    CHECK( b.length== 13 );

    CHECK( c.tagvalue == 3 );
    CHECK( c.length== 65 );
}
TEST_CASE("invaliddata") {
    CHECK_THROWS(asn1tlv(makerange(std::vector<uint8_t>{})));

    // missing length
    CHECK_THROWS(asn1tlv(makerange(std::vector<uint8_t>{0x30})));

    // missing content
    CHECK_NOTHROW(asn1tlv(makerange(std::vector<uint8_t>{0x30,0x01})));

    // missing length part
    CHECK_THROWS(asn1tlv(makerange(std::vector<uint8_t>{0x30,0x81     })));

    // missing content
    CHECK_NOTHROW(asn1tlv(makerange(std::vector<uint8_t>{0x30,0x81,0x80})));

    // missing content part
    CHECK_NOTHROW(asn1tlv(makerange(std::vector<uint8_t>{0x30,0x02,0x05})));

    // missing extented tag
    CHECK_THROWS(asn1tlv(makerange(std::vector<uint8_t>{0x1f     })));

    // missing extented tag part
    CHECK_THROWS(asn1tlv(makerange(std::vector<uint8_t>{0x1f,0x80})));
    // extended tag, missing length
    CHECK_THROWS(asn1tlv(makerange(std::vector<uint8_t>{0x1f,0x80,0x01})));
    // extended tag, missing content
    CHECK_NOTHROW(asn1tlv(makerange(std::vector<uint8_t>{0x1f,0x80,0x01,0x01})));
    // extended tag, missing length part
    CHECK_THROWS(asn1tlv(makerange(std::vector<uint8_t>{0x1f,0x80,0x01,0x81     })));

    // extended tag, missing content
    CHECK_NOTHROW(asn1tlv(makerange(std::vector<uint8_t>{0x1f,0x80,0x01,0x81,0x80})));
    // extended tag, missing content part
    CHECK_NOTHROW(asn1tlv(makerange(std::vector<uint8_t>{0x1f,0x80,0x01,0x02,0x05})));

    auto x = asn1tlv(makerange(std::vector<uint8_t>{0x1f,0x80,0x01,0x00}));

    CHECK(x.tagvalue == 1);
    CHECK(x.length == 0);
}

// todo: get_nth_tlv

#if 0
TEST_CASE("asn1iter2r") {
    auto data = std::vector<uint8_t> { 
0x30,0x82,0x01,0x92,0x30,0x82,0x01,0x3c,0xa0,0x03,0x02,0x01,0x02,0x02,0x10,0x38,0xfe,0x09,0xc9,0xa0,0x00,0xed,0x95,0x11,0xd0,0x2d,0x17,0xd7,0xb4,0x08,0xa0,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x04,0x05,0x00,0x30,0x16,0x31,0x14,0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,0x0b,0x52,0x6f,0x6f,0x74,0x20,0x41,0x67,0x65,0x6e,0x63,0x79,0x30,0x1e,0x17,0x0d,0x39,0x36,0x31,0x30,0x32,0x33,0x32,0x30,0x35,0x36,0x30,0x33,0x5a,0x17,0x0d,0x33,0x39,0x31,0x32,0x33,0x31,0x32,0x33,0x35,0x39,0x35,0x39,0x5a,0x30,0x16,0x31,0x14,0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,0x0b,0x54,0x65,0x73,0x74,0x43,0x6f,0x6d,0x70,0x61,0x6e,0x79,0x30,0x5b,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x01,0x05,0x00,0x03,0x4a,0x00,0x30,0x47,0x02,0x40,0x6b,0x7f,0x91,0x58,0xbd,0x1a,0xf9,0x86,0x37,0x51,0xf1,0x85,0x07,0xf4,0x3a,0x94,0x28,0x17,0xac,0xf8,0x9e,0x21,0xf2,0x0b,0x2f,0x80,0xce,0xaf,0x05,0x60,0x10,0x27,0xdd,0x23,0xfe,0x39,0xc5,0xf9,0x81,0x19,0x2b,0x83,0xb1,0xc4,0x01,0x72,0x9a,0x3c,0x4a,0xea,0xb5,0x09,0x75,0xfa,0x6a,0xa1,0xf8,0xd5,0x43,0x59,0x16,0x5f,0x6e,0x7f,0x02,0x03,0x01,0x00,0x01,0xa3,0x67,0x30,0x65,0x30,0x1a,0x06,0x03,0x55,0x04,0x03,0x04,0x13,0x13,0x11,0x4d,0x69,0x63,0x72,0x6f,0x73,0x6f,0x66,0x74,0x20,0x54,0x72,0x69,0x64,0x65,0x6e,0x74,0x30,0x47,0x06,0x03,0x55,0x1d,0x01,0x04,0x40,0x30,0x3e,0x80,0x10,0x12,0xe4,0x09,0x2d,0x06,0x1d,0x1d,0x4f,0x00,0x8d,0x61,0x21,0xdc,0x16,0x64,0x63,0xa1,0x18,0x30,0x16,0x31,0x14,0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,0x0b,0x52,0x6f,0x6f,0x74,0x20,0x41,0x67,0x65,0x6e,0x63,0x79,0x82,0x10,0x06,0x37,0x6c,0x00,0xaa,0x00,0x64,0x8a,0x11,0xcf,0xb8,0xd4,0xaa,0x5c,0x35,0xf4,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x04,0x05,0x00,0x03,0x41,0x00,0x7a,0x4b,0xfa,0xd7,0x6c,0x8b,0xbf,0x1d,0xa7,0xc5,0xcc,0x64,0xe7,0x9f,0x7d,0x9f,0x76,0x89,0x46,0xa9,0x0b,0xc2,0xf6,0xc6,0x88,0x60,0xc4,0x84,0xe0,0x8f,0xd0,0x4e,0x06,0xdb,0xf4,0xf7,0xde,0x0e,0x30,0x49,0x3a,0xdc,0xdd,0x23,0x03,0x1a,0x3f,0x9f,0xef,0xb8,0x4c,0xea,0x90,0x0c,0x6e,0xf0,0xe8,0x4c,0x20,0x4f,0x37,0xbc,0x1d,0x34
    };

    auto x = asn1tlv(makerange(data));



    auto en = enumtlvs(x.datarange);
    auto p = en.begin();
    auto last = en.end();

    auto& a = *p++;
    auto& b = *p++;
    auto& c = *p++;

    CHECK( p == last );

    CHECK( a.tagvalue == 16 );
    CHECK( a.length== 316 );

    CHECK( b.tagvalue == 16 );
    CHECK( b.length== 13 );

    CHECK( c.tagvalue == 3 );
    CHECK( c.length== 65 );
}
#endif

#if 0
TEST_CASE("asn1parser-stream") {
    auto data = std::vector<uint8_t> { 
0x30,0x82,0x01,0x92,0x30,0x82,0x01,0x3c,0xa0,0x03,0x02,0x01,0x02,0x02,0x10,0x38,0xfe,0x09,0xc9,0xa0,0x00,0xed,0x95,0x11,0xd0,0x2d,0x17,0xd7,0xb4,0x08,0xa0,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x04,0x05,0x00,0x30,0x16,0x31,0x14,0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,0x0b,0x52,0x6f,0x6f,0x74,0x20,0x41,0x67,0x65,0x6e,0x63,0x79,0x30,0x1e,0x17,0x0d,0x39,0x36,0x31,0x30,0x32,0x33,0x32,0x30,0x35,0x36,0x30,0x33,0x5a,0x17,0x0d,0x33,0x39,0x31,0x32,0x33,0x31,0x32,0x33,0x35,0x39,0x35,0x39,0x5a,0x30,0x16,0x31,0x14,0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,0x0b,0x54,0x65,0x73,0x74,0x43,0x6f,0x6d,0x70,0x61,0x6e,0x79,0x30,0x5b,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x01,0x05,0x00,0x03,0x4a,0x00,0x30,0x47,0x02,0x40,0x6b,0x7f,0x91,0x58,0xbd,0x1a,0xf9,0x86,0x37,0x51,0xf1,0x85,0x07,0xf4,0x3a,0x94,0x28,0x17,0xac,0xf8,0x9e,0x21,0xf2,0x0b,0x2f,0x80,0xce,0xaf,0x05,0x60,0x10,0x27,0xdd,0x23,0xfe,0x39,0xc5,0xf9,0x81,0x19,0x2b,0x83,0xb1,0xc4,0x01,0x72,0x9a,0x3c,0x4a,0xea,0xb5,0x09,0x75,0xfa,0x6a,0xa1,0xf8,0xd5,0x43,0x59,0x16,0x5f,0x6e,0x7f,0x02,0x03,0x01,0x00,0x01,0xa3,0x67,0x30,0x65,0x30,0x1a,0x06,0x03,0x55,0x04,0x03,0x04,0x13,0x13,0x11,0x4d,0x69,0x63,0x72,0x6f,0x73,0x6f,0x66,0x74,0x20,0x54,0x72,0x69,0x64,0x65,0x6e,0x74,0x30,0x47,0x06,0x03,0x55,0x1d,0x01,0x04,0x40,0x30,0x3e,0x80,0x10,0x12,0xe4,0x09,0x2d,0x06,0x1d,0x1d,0x4f,0x00,0x8d,0x61,0x21,0xdc,0x16,0x64,0x63,0xa1,0x18,0x30,0x16,0x31,0x14,0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,0x0b,0x52,0x6f,0x6f,0x74,0x20,0x41,0x67,0x65,0x6e,0x63,0x79,0x82,0x10,0x06,0x37,0x6c,0x00,0xaa,0x00,0x64,0x8a,0x11,0xcf,0xb8,0xd4,0xaa,0x5c,0x35,0xf4,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x04,0x05,0x00,0x03,0x41,0x00,0x7a,0x4b,0xfa,0xd7,0x6c,0x8b,0xbf,0x1d,0xa7,0xc5,0xcc,0x64,0xe7,0x9f,0x7d,0x9f,0x76,0x89,0x46,0xa9,0x0b,0xc2,0xf6,0xc6,0x88,0x60,0xc4,0x84,0xe0,0x8f,0xd0,0x4e,0x06,0xdb,0xf4,0xf7,0xde,0x0e,0x30,0x49,0x3a,0xdc,0xdd,0x23,0x03,0x1a,0x3f,0x9f,0xef,0xb8,0x4c,0xea,0x90,0x0c,0x6e,0xf0,0xe8,0x4c,0x20,0x4f,0x37,0xbc,0x1d,0x34
    };

    std::stringstream ss(std::string(data.begin(), data.end()));

    auto x = asn1tlv<std::istream::pos_type>(ss);

    using equalsvector = EqualsVector<decltype(x.datarange)>;

    CHECK( x.constructed == true );
    CHECK( x.cls == 0 );
    CHECK( x.tagvalue == 0x10 );
    CHECK( x.length == 0x192 );

    CHECK_THAT( x.tagrange, equalsvector(0x30));
    CHECK_THAT( x.lenrange, equalsvector(0x82, 0x01,0x92));
    CHECK( x.datarange == makerange(data.begin()+4, data.end()));

    CHECK( traverse(x, { 0, 3, 0 }).length == 13 );
    CHECK_THAT( traverse(x, { 0, 3, 0 }).datarange, equalsvector(0x39,0x36,0x31,0x30,0x32,0x33,0x32,0x30,0x35,0x36,0x30,0x33,0x5a) );

    CHECK( traverse(x, {  }).datarange == makerange(data.begin()+4, data.end()));   // cert+alg+sig body
    CHECK( traverse(x, { 0 }).datarange== makerange(data.begin()+8, data.begin()+8+316));
    CHECK_THAT( traverse(x, { 0, -1 }).datarange, equalsvector(0x02, 0x01, 0x02));  // cert.version
    CHECK_THAT( traverse(x, { 0, 0 }).datarange,    equalsvector(0x38,0xfe,0x09,0xc9,0xa0,0x00,0xed,0x95,0x11,0xd0,0x2d,0x17,0xd7,0xb4,0x08,0xa0)                                                                                                                                                                                                                                                                                                                                                                                                                                         );    // cert.serial
    CHECK_THAT( traverse(x, { 0, 1 }).datarange,    equalsvector(0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x04,0x05,0x00)                                                                                                                                                                                                                                                                                                                                                                                                                                                        );    // cert.algorithm
    CHECK_THAT( traverse(x, { 0, 3, 0 }).datarange, equalsvector(0x39,0x36,0x31,0x30,0x32,0x33,0x32,0x30,0x35,0x36,0x30,0x33,0x5a)                                                                                                                                                                                                                                                                                                                                                                                                                                                        );  // first timestamp
    CHECK_THAT( traverse(x, { 0, 4, 0 }).datarange, equalsvector(0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,0x0b,0x54,0x65,0x73,0x74,0x43,0x6f,0x6d,0x70,0x61,0x6e,0x79)                                                                                                                                                                                                                                                                                                                                                                                                                     );  // subject
    CHECK_THAT( traverse(x, { 0, -4, 0 }).datarange,equalsvector(0x30,0x1a,0x06,0x03,0x55,0x04,0x03,0x04,0x13,0x13,0x11,0x4d,0x69,0x63,0x72,0x6f,0x73,0x6f,0x66,0x74,0x20,0x54,0x72,0x69,0x64,0x65,0x6e,0x74,0x30,0x47,0x06,0x03,0x55,0x1d,0x01,0x04,0x40,0x30,0x3e,0x80,0x10,0x12,0xe4,0x09,0x2d,0x06,0x1d,0x1d,0x4f,0x00,0x8d,0x61,0x21,0xdc,0x16,0x64,0x63,0xa1,0x18,0x30,0x16,0x31,0x14,0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,0x0b,0x52,0x6f,0x6f,0x74,0x20,0x41,0x67,0x65,0x6e,0x63,0x79,0x82,0x10,0x06,0x37,0x6c,0x00,0xaa,0x00,0x64,0x8a,0x11,0xcf,0xb8,0xd4,0xaa,0x5c,0x35,0xf4));    // cert.extensions
    CHECK_THAT( traverse(x, { 1 }).datarange,       equalsvector(0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x04,0x05,0x00)                                                                                                                                                                                                                                                                                                                                                                                                                                                        );         // sig.algorithm
}
#endif

