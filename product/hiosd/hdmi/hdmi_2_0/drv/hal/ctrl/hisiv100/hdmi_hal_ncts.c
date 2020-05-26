/*
* Copyright (c) 2018 HiSilicon Technologies Co., Ltd.
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/

#include "hdmi_hal_ncts.h"
#include "hdmi_hal_intf.h"
#include "hdmi_product_define.h"

static HDMI_AUDIO_CTS_N_S s_astAudioCtsN[] =
{
/* Rate=32k ,       TMDS  ,        N      ,          CTS    , */
    { 32000,     25174 ,     4576 ,      28125 },
    { 32000,     25200 ,     4096 ,      25200 },
    { 32000,     27000 ,     4096 ,      27000 },
    { 32000,     27027 ,     4096 ,      27027 },
    { 32000,     54000 ,     4096 ,      54000 },
    { 32000,     54054 ,     4096 ,      54054 },
    { 32000,     74175 ,     11648,      210937},
    { 32000,     74250 ,     4096 ,      74250 },
    { 32000,     148351,     11648,      421875},
    { 32000,     148500,     4096 ,      148500},
    { 32000,     296703,     5824 ,      421875},
    { 32000,     297000,     3072 ,      222750},
    { 32000,     593406,     5824 ,      843750},
    { 32000,     594000,     3072 ,      445500},
    { 32000,     0     ,      4096 ,      0},      /* Measured */

/* Rate=44.1k ,    TMDS  ,        N      ,          CTS    , */
    { 44100,     25174 ,     7007 ,      31250 },
    { 44100,     25200 ,     6272 ,      28000 },
    { 44100,     27000 ,     6272 ,      30000 },
    { 44100,     27027 ,     6272 ,      30030 },
    { 44100,     54000 ,     6272 ,      60000 },
    { 44100,     54054 ,     6272 ,      60060 },
    { 44100,     74175 ,     17836,      234375},
    { 44100,     74250 ,     6272 ,      82500 },
    { 44100,     148351,     8918 ,      234375},
    { 44100,     148500,     6272 ,      165000},
    { 44100,     296703,     4459 ,      234375},
    { 44100,     297000,     4704 ,      247500},
    { 44100,     593406,     8918 ,      937500},
    { 44100,     594000,     9408 ,      990000},
    { 44100,     0     ,      6272 ,      0},      /* Measured */

/* Rate=48k ,      TMDS  ,        N      ,          CTS    , */
    {48000,     25174 ,     6864 ,      28125 },
    {48000,     25200 ,     6144 ,      25200 },
    {48000,     27000 ,     6144 ,      27000 },
    {48000,     27027 ,     6144 ,      27027 },
    {48000,     54000 ,     6144 ,      54000 },
    {48000,     54054 ,     6144 ,      54054 },
    {48000,     74175 ,     11648,      140625},
    {48000,     74250 ,     6144 ,      74250 },
    {48000,     148351,     5824 ,      140625},
    {48000,     148500,     6144 ,      148500},
    {48000,     296703,     5824 ,      281250},
    {48000,     297000,     5120 ,      247500},
    {48000,     593406,     5824 ,      562500},
    {48000,     594000,     6144 ,      594000},
    {48000,     0     ,      6144 ,      0},      /* Measured */

#ifndef CONFIG_HDMI_BVT_SDK
/* Rate=64k ,       TMDS  ,        N      ,          CTS    , */
    { 64000,     25174 ,     9152 ,      28125 },
    { 64000,     25200 ,     8192 ,      25200 },
    { 64000,     27000 ,     8192 ,      27000 },
    { 64000,     27027 ,     8192 ,      27027 },
    { 64000,     54000 ,     8192 ,      54000 },
    { 64000,     54054 ,     8192 ,      54054 },
    { 64000,     74175 ,     23296,      210937},
    { 64000,     74250 ,     8192 ,      74250 },
    { 64000,     148351,     23296,      421875},
    { 64000,     148500,     8192 ,      148500},
    { 64000,     296703,     11648 ,     421875},
    { 64000,     297000,     8192 ,      297000},
    { 64000,     593406,     11648,      843750},
    { 64000,     594000,     8192 ,      594000},
    { 64000,     0     ,      8192 ,      0},      /* Measured */

/* Rate=88.2k ,     TMDS  ,        N      ,          CTS    , */
    { 88200,     25174 ,     14014,      31250 },
    { 88200,     25200 ,     12544,      28000 },
    { 88200,     27000 ,     12544,      30000 },
    { 88200,     27027 ,     12544,      30030 },
    { 88200,     54000 ,     12544,      60000 },
    { 88200,     54054 ,     12544,      60060 },
    { 88200,     74175 ,     35672,      234375},
    { 88200,     74250 ,     12544,      82500 },
    { 88200,     148351,     17836,      234375},
    { 88200,     148500,     12544,      165000},
    { 88200,     296703,     8918 ,      234375},
    { 88200,     297000,     9408 ,      247500},
    { 88200,     593406,     17836,      937500},
    { 88200,     594000,     18816,      990000},
    { 88200,     0     ,      12544,      0},      /* Measured */

/* Rate=96k ,     TMDS  ,        N      ,          CTS    , */
    {96000,     25174 ,     13728,      28125 },
    {96000,     25200 ,     12288,      25200 },
    {96000,     27000 ,     12288,      27000 },
    {96000,     27027 ,     12288,      27027 },
    {96000,     54000 ,     12288,      54000 },
    {96000,     54054 ,     12288,      54054 },
    {96000,     74175 ,     23296,      140625},
    {96000,     74250 ,     12288,      74250 },
    {96000,     148351,     11648,      140625},
    {96000,     148500,     12288,      148500},
    {96000,     296703,     11648,      281250},
    {96000,     297000,     10240,      247500},
    {96000,     593406,     11648 ,     562500},
    {96000,     594000,     12288 ,     594000},
    {96000,     0     ,      12288,      0},      /* Measured */

/* Rate=128k ,      TMDS  ,        N      ,          CTS    , */
    {128000,     25174 ,     18304 ,      28125 },
    {128000,     25200 ,     16384 ,      25200 },
    {128000,     27000 ,     16384 ,      27000 },
    {128000,     27027 ,     16384 ,      27027 },
    {128000,     54000 ,     16384 ,      54000 },
    {128000,     54054 ,     16384 ,      54054 },
    {128000,     74175 ,     46592 ,      210937},
    {128000,     74250 ,     16384 ,      74250 },
    {128000,     148351,     46592 ,      421875},
    {128000,     148500,     16384 ,      148500},
    {128000,     296703,     23296 ,      421875},
    {128000,     297000,     16384 ,      297000},
    {128000,     593406,     23296 ,      843750},
    {128000,     594000,     16384 ,      594000},
    {128000,     0     ,      16384 ,      0},      /* Measured */

/* Rate=176.4k,   TMDS  ,        N      ,          CTS    , */
    {176400,     25174 ,     28028,      31250 },
    {176400,     25200 ,     25088,      28000 },
    {176400,     27000 ,     25088,      27000 },
    {176400,     27027 ,     25088,      30000 },
    {176400,     54000 ,     25088,      30030 },
    {176400,     54054 ,     25088,      60000 },
    {176400,     74175 ,     71344,      60060 },
    {176400,     74250 ,     25088,      334375},
    {176400,     148351,     35672,      82500 },
    {176400,     148500,     25088,      165000},
    {176400,     296703,     17836,      234375},
    {176400,     297000,     18816,      247500},
    {176400,     593406,     35672,      937500},
    {176400,     594000,     27632,      990000},
    {176400,     0     ,      25088,      0},      /* Measured */

 /* Rate=192k ,     TMDS  ,        N      ,          CTS    , */
    {192000,     25174 ,     27456,      28125 },
    {192000,     25200 ,     24576,      25200 },
    {192000,     27000 ,     24576,      27000 },
    {192000,     27027 ,     24576,      27027 },
    {192000,     54000 ,     24576,      54000 },
    {192000,     54054 ,     24576,      54054 },
    {192000,     74175 ,     46592,      140625},
    {192000,     74250 ,     24576,      74250 },
    {192000,     148351,     23296,      140625},
    {192000,     148500,     24576,      148500},
    {192000,     296703,     23296,      281250},
    {192000,     297000,     20480,      247500},
    {192000,     593406,     23296 ,     562500},
    {192000,     594000,     24576 ,     594000},
    {192000,     0     ,      24576,      0},      /* Measured */

/* Rate=256k ,       TMDS  ,        N      ,          CTS    , */
    {256000,     25174 ,     36608 ,      28125 },
    {256000,     25200 ,     32768 ,      25200 },
    {256000,     27000 ,     32768 ,      27000 },
    {256000,     27027 ,     32768 ,      27027 },
    {256000,     54000 ,     32768 ,      54000 },
    {256000,     54054 ,     32768 ,      54054 },
    {256000,     74175 ,     46592 ,      210937},
    {256000,     74250 ,     32768 ,      74250 },
    {256000,     148351,     46592 ,      421875},
    {256000,     148500,     32768 ,      148500},
    {256000,     296703,     23296 ,      421875},
    {256000,     297000,     32768 ,      297000},
    {256000,     593406,     23296 ,      843750},
    {256000,     594000,     32768 ,      594000},
    {256000,     0     ,      32768 ,      0},      /* Measured */

/* Rate=352.8k,   TMDS  ,        N      ,          CTS    , */
    {352800,     25174 ,     56056,      31250 },
    {352800,     25200 ,     50176,      28000 },
    {352800,     27000 ,     50176,      27000 },
    {352800,     27027 ,     50176,      30000 },
    {352800,     54000 ,     50176,      30030 },
    {352800,     54054 ,     50176,      60000 },
    {352800,     74175 ,    142688,      60060 },
    {352800,     74250 ,     50176,      334375},
    {352800,     148351,     71344,      82500 },
    {352800,     148500,     50176,      165000},
    {352800,     296703,     35672,      234375},
    {352800,     297000,     37632,      247500},
    {352800,     593406,     71344,      937500},
    {352800,     594000,     75264,      990000},
    {352800,     0     ,      50176,      0},      /* Measured */

 /* Rate=384k ,     TMDS  ,        N      ,          CTS    , */
    {384000,     25174 ,     54912,      28125 },
    {384000,     25200 ,     49152,      25200 },
    {384000,     27000 ,     49152,      27000 },
    {384000,     27027 ,     49152,      27027 },
    {384000,     54000 ,     49152,      54000 },
    {384000,     54054 ,     49152,      54054 },
    {384000,     74175 ,     93184,      140625},
    {384000,     74250 ,     49152,      74250 },
    {384000,     148351,     46592,      140625},
    {384000,     148500,     49152,      148500},
    {384000,     296703,     46592,      281250},
    {384000,     297000,     40960,      247500},
    {384000,     593406,     46592 ,     562500},
    {384000,     594000,     49152 ,     594000},
    {384000,     0     ,      49152,      0},      /* Measured */
#endif
};

#define NCTS_SIZE (sizeof(s_astAudioCtsN)/sizeof(s_astAudioCtsN[0]))
#define NCTS_TMDS_DEVIATION     20
#define NCTS_INVALID_DATA       0xffffffff

HI_U32 HAL_HDMI_NValueGet(HI_U32 u32SampleRate,HI_U32 u32TmdsClk)
{
    HI_U32              i = 0;
    HDMI_AUDIO_CTS_N_S *pstAudioCtsN = &s_astAudioCtsN[0];
    HI_U32              u32NValue = 6144;

    for (i = 0;i < NCTS_SIZE; i++,pstAudioCtsN++)
    {
        if ( (pstAudioCtsN->u32AudioSmpRate == u32SampleRate) && (pstAudioCtsN->u32TmdsClk == u32TmdsClk) )
        {
            u32NValue = pstAudioCtsN->u32NValue;
            break;
        }
        else if ( (pstAudioCtsN->u32AudioSmpRate == u32SampleRate)
            && (pstAudioCtsN->u32TmdsClk >= (u32TmdsClk - NCTS_TMDS_DEVIATION))
            && (pstAudioCtsN->u32TmdsClk <= (u32TmdsClk + NCTS_TMDS_DEVIATION)) )
        {
            u32NValue = pstAudioCtsN->u32NValue;
            break;
        }
        else if ( (pstAudioCtsN->u32AudioSmpRate == u32SampleRate) && (pstAudioCtsN->u32TmdsClk == 0) )
        {
            u32NValue = pstAudioCtsN->u32NValue;
            break;
        }
    }

    return u32NValue;

}

HI_U32 HAL_HDMI_CtsValueGet(HI_U32 u32SampleRate,HI_U32 u32TmdsClk)
{
    HI_U32              i = 0;
    HI_U32              u32TmpCts = NCTS_INVALID_DATA;
    HI_U32              u32TmpN = NCTS_INVALID_DATA;
    HDMI_AUDIO_CTS_N_S *pstAudioCtsN = &s_astAudioCtsN[0];

    for (i = 0;i < NCTS_SIZE; i++,pstAudioCtsN++)
    {
        if ( (pstAudioCtsN->u32AudioSmpRate == u32SampleRate) && (pstAudioCtsN->u32TmdsClk == u32TmdsClk) )
        {
            u32TmpCts = pstAudioCtsN->u32CtsValue;
            u32TmpN = pstAudioCtsN->u32NValue;
            break;
        }
        else if ( (pstAudioCtsN->u32AudioSmpRate == u32SampleRate)
            && (pstAudioCtsN->u32TmdsClk >= (u32TmdsClk - NCTS_TMDS_DEVIATION))
            && (pstAudioCtsN->u32TmdsClk <= (u32TmdsClk + NCTS_TMDS_DEVIATION)) )
        {
            u32TmpCts = pstAudioCtsN->u32CtsValue;
            u32TmpN = pstAudioCtsN->u32NValue;
            break;
        }
        else if ( (pstAudioCtsN->u32AudioSmpRate == u32SampleRate) && (pstAudioCtsN->u32TmdsClk == 0) )
        {
            u32TmpCts = pstAudioCtsN->u32CtsValue;
            u32TmpN = pstAudioCtsN->u32NValue;
            break;
        }
    }

    /* Measured */
    if (u32TmpCts == 0 && u32SampleRate)
    {
        u32TmpCts = (u32TmpN/128) * u32TmdsClk * 10 / (u32SampleRate / 100);
    }
    else if (u32TmpCts == NCTS_INVALID_DATA && u32SampleRate) /* can't find , N default 48k samprate */
    {
        HDMI_WARN("can't find cts! u32TmpCts=%d,u32SampleRate=%d,u32TmdsClk=%d\n",u32TmpCts,u32SampleRate,u32TmdsClk);
        u32TmpCts = (6144/128) * u32TmdsClk * 10 / (u32SampleRate / 100);
    }

    return u32TmpCts;
}


