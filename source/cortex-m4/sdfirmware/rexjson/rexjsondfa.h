/*
 *  This file is auto generated by rexcc compiler from the RPA/Tk project
 */

/*
 * To generate the rexjsondfa.h file from rexjsondfa.rexcc do:
 * # rexcc rexjsondfa.rexcc -o rexjsondfa.h
 */

#include "rexdfatypes.h"

#define TOKEN_SELF 256
#define TOKEN_SPACE 257
#define TOKEN_STRING 261
#define TOKEN_INT 262
#define TOKEN_NUMBER 263
#define TOKEN_TRUE	264
#define TOKEN_FALSE 266
#define TOKEN_NULL 267
#define TOKEN_COMMA 268
#define TOKEN_COLON 269
#define TOKEN_LEFTSB 270
#define TOKEN_RIGHTSB 271
#define TOKEN_LEFTCB 272
#define TOKEN_RIGHTCB 273


static rexdfss_t accsubstates[] = {
	{                2,               71, (rexuserdata_t)(TOKEN_SPACE) },
	{                2,               58, (rexuserdata_t)(TOKEN_COMMA) },
	{                2,               21, (rexuserdata_t)(TOKEN_INT) },
	{                2,               40, (rexuserdata_t)(TOKEN_NUMBER) },
	{                2,               21, (rexuserdata_t)(TOKEN_INT) },
	{                2,               40, (rexuserdata_t)(TOKEN_NUMBER) },
	{                2,               60, (rexuserdata_t)(TOKEN_COLON) },
	{                2,               62, (rexuserdata_t)(TOKEN_LEFTSB) },
	{                2,               64, (rexuserdata_t)(TOKEN_RIGHTSB) },
	{                2,               66, (rexuserdata_t)(TOKEN_LEFTCB) },
	{                2,               68, (rexuserdata_t)(TOKEN_RIGHTCB) },
	{                2,               12, (rexuserdata_t)(TOKEN_STRING) },
	{                2,               21, (rexuserdata_t)(TOKEN_INT) },
	{                2,               40, (rexuserdata_t)(TOKEN_NUMBER) },
	{                2,               40, (rexuserdata_t)(TOKEN_NUMBER) },
	{                2,               40, (rexuserdata_t)(TOKEN_NUMBER) },
	{                2,               56, (rexuserdata_t)(TOKEN_NULL) },
	{                2,               45, (rexuserdata_t)(TOKEN_TRUE) },
	{                2,               51, (rexuserdata_t)(TOKEN_FALSE) },
	{                0,                0,                0 },
};


static rexdfss_t substates[] = {
	{                0,                0,                0 },
};


static rexdft_t transitions[] = {
	{                0,       4294967295,                0 },
	{                0,                8,                0 },
	{                9,               10,                2 },
	{               11,               12,                0 },
	{               13,               13,                2 },
	{               14,               31,                0 },
	{               32,               32,                2 },
	{               33,               33,                0 },
	{               34,               34,                3 },
	{               35,               43,                0 },
	{               44,               44,                4 },
	{               45,               45,                5 },
	{               46,               47,                0 },
	{               48,               48,                6 },
	{               49,               57,                7 },
	{               58,               58,                8 },
	{               59,               90,                0 },
	{               91,               91,                9 },
	{               92,               92,                0 },
	{               93,               93,               10 },
	{               94,              101,                0 },
	{              102,              102,               11 },
	{              103,              109,                0 },
	{              110,              110,               12 },
	{              111,              115,                0 },
	{              116,              116,               13 },
	{              117,              122,                0 },
	{              123,              123,               14 },
	{              124,              124,                0 },
	{              125,              125,               15 },
	{              126,       4294967295,                0 },
	{                0,                8,                0 },
	{                9,               10,                2 },
	{               11,               12,                0 },
	{               13,               13,                2 },
	{               14,               31,                0 },
	{               32,               32,                2 },
	{               33,       4294967295,                0 },
	{                0,               31,                0 },
	{               32,               33,                3 },
	{               34,               34,               16 },
	{               35,               91,                3 },
	{               92,               92,               17 },
	{               93,       4294967295,                3 },
	{                0,       4294967295,                0 },
	{                0,               47,                0 },
	{               48,               48,                6 },
	{               49,               57,                7 },
	{               58,       4294967295,                0 },
	{                0,               44,                0 },
	{               45,               45,               18 },
	{               46,               46,               19 },
	{               47,               68,                0 },
	{               69,               69,               20 },
	{               70,              100,                0 },
	{              101,              101,               20 },
	{              102,       4294967295,                0 },
	{                0,               44,                0 },
	{               45,               45,               18 },
	{               46,               46,               19 },
	{               47,               47,                0 },
	{               48,               57,               21 },
	{               58,               68,                0 },
	{               69,               69,               20 },
	{               70,              100,                0 },
	{              101,              101,               20 },
	{              102,       4294967295,                0 },
	{                0,       4294967295,                0 },
	{                0,       4294967295,                0 },
	{                0,       4294967295,                0 },
	{                0,               96,                0 },
	{               97,               97,               22 },
	{               98,       4294967295,                0 },
	{                0,              116,                0 },
	{              117,              117,               23 },
	{              118,       4294967295,                0 },
	{                0,              113,                0 },
	{              114,              114,               24 },
	{              115,       4294967295,                0 },
	{                0,       4294967295,                0 },
	{                0,       4294967295,                0 },
	{                0,       4294967295,                0 },
	{                0,               33,                0 },
	{               34,               34,                3 },
	{               35,               91,                0 },
	{               92,               92,                3 },
	{               93,               97,                0 },
	{               98,               98,                3 },
	{               99,              101,                0 },
	{              102,              102,                3 },
	{              103,              109,                0 },
	{              110,              110,                3 },
	{              111,              113,                0 },
	{              114,              114,                3 },
	{              115,              115,                0 },
	{              116,              116,                3 },
	{              117,              117,               25 },
	{              118,       4294967295,                0 },
	{                0,               68,                0 },
	{               69,               69,               20 },
	{               70,              100,                0 },
	{              101,              101,               20 },
	{              102,       4294967295,                0 },
	{                0,               47,                0 },
	{               48,               57,               26 },
	{               58,       4294967295,                0 },
	{                0,               42,                0 },
	{               43,               43,               27 },
	{               44,               44,                0 },
	{               45,               45,               27 },
	{               46,               47,                0 },
	{               48,               57,               28 },
	{               58,       4294967295,                0 },
	{                0,               44,                0 },
	{               45,               45,               18 },
	{               46,               46,               19 },
	{               47,               47,                0 },
	{               48,               57,               21 },
	{               58,               68,                0 },
	{               69,               69,               20 },
	{               70,              100,                0 },
	{              101,              101,               20 },
	{              102,       4294967295,                0 },
	{                0,              107,                0 },
	{              108,              108,               29 },
	{              109,       4294967295,                0 },
	{                0,              107,                0 },
	{              108,              108,               30 },
	{              109,       4294967295,                0 },
	{                0,              116,                0 },
	{              117,              117,               31 },
	{              118,       4294967295,                0 },
	{                0,               47,                0 },
	{               48,               57,               32 },
	{               58,               64,                0 },
	{               65,               70,               32 },
	{               71,               96,                0 },
	{               97,              102,               32 },
	{              103,       4294967295,                0 },
	{                0,               44,                0 },
	{               45,               45,               18 },
	{               46,               47,                0 },
	{               48,               57,               26 },
	{               58,               68,                0 },
	{               69,               69,               20 },
	{               70,              100,                0 },
	{              101,              101,               20 },
	{              102,       4294967295,                0 },
	{                0,               47,                0 },
	{               48,               57,               28 },
	{               58,       4294967295,                0 },
	{                0,               47,                0 },
	{               48,               57,               28 },
	{               58,       4294967295,                0 },
	{                0,              114,                0 },
	{              115,              115,               33 },
	{              116,       4294967295,                0 },
	{                0,              107,                0 },
	{              108,              108,               34 },
	{              109,       4294967295,                0 },
	{                0,              100,                0 },
	{              101,              101,               35 },
	{              102,       4294967295,                0 },
	{                0,               47,                0 },
	{               48,               57,               36 },
	{               58,               64,                0 },
	{               65,               70,               36 },
	{               71,               96,                0 },
	{               97,              102,               36 },
	{              103,       4294967295,                0 },
	{                0,              100,                0 },
	{              101,              101,               37 },
	{              102,       4294967295,                0 },
	{                0,       4294967295,                0 },
	{                0,       4294967295,                0 },
	{                0,               47,                0 },
	{               48,               57,               38 },
	{               58,               64,                0 },
	{               65,               70,               38 },
	{               71,               96,                0 },
	{               97,              102,               38 },
	{              103,       4294967295,                0 },
	{                0,       4294967295,                0 },
	{                0,               47,                0 },
	{               48,               57,                3 },
	{               58,               64,                0 },
	{               65,               70,                3 },
	{               71,               96,                0 },
	{               97,              102,                3 },
	{              103,       4294967295,                0 },
	{                0,                0,                0 },
};


static rexdfs_t states[] = {
	{                3,                0,                1,                0,                0,                0 ,                0},
	{                1,                1,               30,                0,                0,                0 ,                0},
	{                2,               31,                7,                0,                1,                0 ,                0},
	{                0,               38,                6,                1,                0,                0 ,                0},
	{                2,               44,                1,                1,                1,                0 ,                0},
	{                0,               45,                4,                2,                0,                0 ,                0},
	{                2,               49,                8,                2,                2,                0 ,                0},
	{                2,               57,               10,                4,                2,                0 ,                0},
	{                2,               67,                1,                6,                1,                0 ,                0},
	{                2,               68,                1,                7,                1,                0 ,                0},
	{                2,               69,                1,                8,                1,                0 ,                0},
	{                0,               70,                3,                9,                0,                0 ,                0},
	{                0,               73,                3,                9,                0,                0 ,                0},
	{                0,               76,                3,                9,                0,                0 ,                0},
	{                2,               79,                1,                9,                1,                0 ,                0},
	{                2,               80,                1,               10,                1,                0 ,                0},
	{                2,               81,                1,               11,                1,                0 ,                0},
	{                0,               82,               16,               12,                0,                0 ,                0},
	{                0,               98,                5,               12,                0,                0 ,                0},
	{                0,              103,                3,               12,                0,                0 ,                0},
	{                0,              106,                7,               12,                0,                0 ,                0},
	{                2,              113,               10,               12,                2,                0 ,                0},
	{                0,              123,                3,               14,                0,                0 ,                0},
	{                0,              126,                3,               14,                0,                0 ,                0},
	{                0,              129,                3,               14,                0,                0 ,                0},
	{                0,              132,                7,               14,                0,                0 ,                0},
	{                2,              139,                9,               14,                1,                0 ,                0},
	{                0,              148,                3,               15,                0,                0 ,                0},
	{                2,              151,                3,               15,                1,                0 ,                0},
	{                0,              154,                3,               16,                0,                0 ,                0},
	{                0,              157,                3,               16,                0,                0 ,                0},
	{                0,              160,                3,               16,                0,                0 ,                0},
	{                0,              163,                7,               16,                0,                0 ,                0},
	{                0,              170,                3,               16,                0,                0 ,                0},
	{                2,              173,                1,               16,                1,                0 ,                0},
	{                2,              174,                1,               17,                1,                0 ,                0},
	{                0,              175,                7,               18,                0,                0 ,                0},
	{                2,              182,                1,               18,                1,                0 ,                0},
	{                0,              183,                7,               19,                0,                0 ,                0},
	{                0,                0,                0,                0,                0,                0 ,                0},
};


static rexdfa_t ccdfa = {
	39,
	states,
	190,
	transitions,
	19,
	accsubstates,
	0,
	substates,
	0,
	0,
	0,
	0,
	{0, },
};


static rexdfa_t *dfa = &ccdfa;
