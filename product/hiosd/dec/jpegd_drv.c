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
#include <common.h>
#include "jpegd_drv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* Begin of #ifdef __cplusplus */

#define osal_readl(x) (*((volatile int*)(x)))
#define osal_writel(v, x) (*((volatile int*)(x)) = (v))

__inline static unsigned int get_low_addr(hi_u64 phyaddr)
{
    return (unsigned int)phyaddr;
}

__inline static unsigned int get_high_addr(hi_u64 phyaddr)
{
    return (unsigned int)(phyaddr >> 32);
}

void jpegd_set_clock_en(int vpu_id, hi_bool is_run)
{
    unsigned long t, mask;
    unsigned int offset = 1;
    mask = 1 << offset;
    t = osal_readl((const volatile void *)JPEGD_CRG_REG_ADDR);
    t &= ~mask;
    t |= ((unsigned int)is_run << offset) & mask;
    osal_writel(t, (volatile void *)JPEGD_CRG_REG_ADDR);

    return;
}

void jpegd_reset_select(int vpu_id, hi_bool is_run)
{
    int i;
    volatile U_JPEG_DEC_START D32;
    S_JPGD_REGS_TYPE *jpegd_reg = (S_JPGD_REGS_TYPE *)jpegd_get_reg_addr(vpu_id);

    if (is_run == HI_FALSE) {
        for (i = 0; i < 1000; i++) {
            D32.u32 = jpegd_reg->JPEG_DEC_START.u32;
            if (D32.bits.rst_busy == 0) {
                break;
            }
            udelay(10);
        }
        if (i >= 1000) {
            printf("JPEGD reset error!\n");
            return;
        }
    }

    {
        unsigned long t, mask;
        unsigned int offset = 0;
        mask = 1 << offset;
        t = osal_readl((const volatile void *)JPEGD_CRG_REG_ADDR);
        t &= ~mask;
        t |= ((unsigned int)is_run << offset) & mask;
        osal_writel(t, (volatile void *)JPEGD_CRG_REG_ADDR);
    }

    return;
}

void jpegd_set_outstanding(int vpu_id, int outstanding)
{
    U_FREQ_SCALE D32;
    S_JPGD_REGS_TYPE *jpegd_reg = (S_JPGD_REGS_TYPE *)jpegd_get_reg_addr(vpu_id);

    D32.u32 = jpegd_reg->FREQ_SCALE.u32;
    D32.bits.outstanding = outstanding;
    jpegd_reg->FREQ_SCALE.u32 = D32.u32;
    return;
}

void *jpegd_get_reg_addr(int vpu_id)
{
    S_JPGD_REGS_TYPE *jpegd_reg = HI_NULL;

    jpegd_reg = (S_JPGD_REGS_TYPE *)JPEGD_REGS_ADDR;

    return jpegd_reg;
}

unsigned int jpegd_read_int(int vpu_id)
{
    S_JPGD_REGS_TYPE *jpegd_reg = (S_JPGD_REGS_TYPE *)jpegd_get_reg_addr(vpu_id);

    return jpegd_reg->JPEG_INT.u32;
}

void jpegd_clear_int(int vpu_id)
{
    S_JPGD_REGS_TYPE *jpegd_reg = (S_JPGD_REGS_TYPE *)jpegd_get_reg_addr(vpu_id);
    /* check if it's frame end */
    jpegd_reg->JPEG_INT.u32 = 0xFFFFFFFF;

    return;
}

void jpegd_set_int_mask(int vpu_id)
{
    S_JPGD_REGS_TYPE *jpegd_reg = (S_JPGD_REGS_TYPE *)jpegd_get_reg_addr(vpu_id);

    jpegd_reg->INT_MASK.u32 = 0x0;

    return;
}

void jpegd_reset(int vpu_id, void *info)
{
    jpegd_reset_select(vpu_id, HI_TRUE);
    jpegd_reset_select(vpu_id, HI_FALSE);
    jpegd_set_int_mask(vpu_id);
    jpegd_set_outstanding(vpu_id, 0x3);

    return;
}

void jpegd_start_vpu(int vpu_id)
{
    U_JPEG_DEC_START D32;
    S_JPGD_REGS_TYPE *jpegd_reg = (S_JPGD_REGS_TYPE *)jpegd_get_reg_addr(vpu_id);

    D32.u32 = jpegd_reg->JPEG_DEC_START.u32;
    D32.bits.jpeg_dec_start = 1;
    jpegd_reg->JPEG_DEC_START.u32 = D32.u32;

    return;
}

int jpegd_set_time_out(int vpu_id, int time_out)
{
    S_JPGD_REGS_TYPE *jpegd_reg = (S_JPGD_REGS_TYPE *)jpegd_get_reg_addr(vpu_id);

    jpegd_reg->OVER_TIME_THD = (unsigned int)time_out;

    return HI_SUCCESS;
}

void jpegd_drv_write_regs(void *reg_base, jpegd_vpu_config *config)
{
    S_JPGD_REGS_TYPE *jpegd_reg = (S_JPGD_REGS_TYPE *)reg_base;

    /*  1. JPEG_RESUME_START */
    {
        U_JPEG_RESUME_START D32;

        D32.u32 = 0;

        D32.bits.jpeg_resume_start = 0;
        D32.bits.last_resume_in_pic = 1;

        jpegd_reg->JPEG_RESUME_START.u32 = D32.u32;
    }

    /*  2. JPEG_STRIDE */
    {
        U_JPEG_STRIDE D32;

        D32.u32 = 0;

        D32.bits.y_stride = config->y_stride;
        D32.bits.uv_stride = config->c_stride;

        jpegd_reg->JPEG_STRIDE.u32 = D32.u32;
    }

    /*  3. PICTURE_SIZE */
    {
        U_PICTURE_SIZE D32;

        D32.u32 = 0;

        D32.bits.pic_width_in_mcu = config->width_in_mcu;
        D32.bits.pic_height_in_mcu = config->height_in_mcu;

        jpegd_reg->PICTURE_SIZE.u32 = D32.u32;
    }

    /*  4. PICTURE_TYPE */
    {
        U_PICTURE_TYPE D32;

        D32.u32 = 0;

        D32.bits.pic_type = config->pic_type;

        jpegd_reg->PICTURE_TYPE.u32 = D32.u32;
    }

    /*  . RGB_OUT_STRIDE */
    {
        U_RGB_OUT_STRIDE D32;

        D32.u32 = 0;

        D32.bits.jcfg2jdo_out_stride = config->rgb_stride;

        jpegd_reg->RGB_OUT_STRIDE.u32 = D32.u32;
    }

    /*  5. BITBUFFER_STADDR */
    {
        jpegd_reg->BITBUFFER_STADDR_LSB = get_low_addr(config->phy_str_buf_start);

        jpegd_reg->BITBUFFER_STADDR_MSB.u32 = get_high_addr(config->phy_str_buf_start);
    }

    /*  6. BITBUFFER_ENDADDR */
    {
        jpegd_reg->BITBUFFER_ENDADDR_LSB = get_low_addr(config->phy_str_buf_end);

        jpegd_reg->BITBUFFER_ENDADDR_MSB.u32 = get_high_addr(config->phy_str_buf_end);
    }

    /*  7. BITSTREAMS_STADDR */
    {
        jpegd_reg->BITSTREAMS_STADDR_LSB = get_low_addr(config->phy_str_start);

        jpegd_reg->BITSTREAMS_STADDR_MSB.u32 = get_high_addr(config->phy_str_start);
    }

    /*  8. BITSTREAMS_ENDADDR */
    {
        jpegd_reg->BITSTREAMS_ENDADDR_LSB = get_low_addr(config->phy_str_end);

        jpegd_reg->BITSTREAMS_ENDADDR_MSB.u32 = get_high_addr(config->phy_str_end);
    }

    /*  9. PICTURE_YSTADDR */
    {
        jpegd_reg->PICTURE_YSTADDR_LSB = get_low_addr(config->y_phy_addr);

        jpegd_reg->PICTURE_YSTADDR_MSB.u32 = get_high_addr(config->y_phy_addr);
    }

    /*  10. PICTURE_UVSTADDR */
    if (config->out_yuv == HI_TRUE) {
        jpegd_reg->PICTURE_UVSTADDR_LSB = get_low_addr(config->c_phy_addr);

        jpegd_reg->PICTURE_UVSTADDR_MSB.u32 = get_high_addr(config->c_phy_addr);
    }

    /*  11. FREQ_SCALE */
    {
        U_FREQ_SCALE D32;
        D32.u32 = jpegd_reg->FREQ_SCALE.u32;

        D32.bits.freq_scale = 0;
        D32.bits.ck_gt_en = 0;
        D32.bits.outstanding = 3;
        D32.bits.axi_id = 0;
        if (config->out_yuv == HI_TRUE) {
            D32.bits.jidct_emar_en = 1;
        } else {
            D32.bits.jidct_emar_en = 0;
        }
        jpegd_reg->FREQ_SCALE.u32 = D32.u32;
    }

    /*  12. OUT_TYPE */
    {
        U_OUT_TYPE D32;

        D32.u32 = 0;

        if (config->out_yuv == HI_TRUE) {
            if (config->pic_format == PICTURE_FORMAT_YUV400) {
                D32.bits.jidct_yuv420_en = 0;
            } else {
                D32.bits.jidct_yuv420_en = 1;
            }
        } else {
            D32.bits.jidct_yuv420_en = 0;
            D32.bits.hor_med_en = 1;
            D32.bits.ver_med_en = 1;
            D32.bits.dither_en = 1;

            D32.bits.hor_med_en = 0;
            D32.bits.ver_med_en = 0;
            D32.bits.dither_en = 0;
            if (config->pixel_format == PIXEL_FORMAT_ARGB_8888) {
                D32.bits.out_rgb_type = 0;
            } else if (config->pixel_format == PIXEL_FORMAT_ABGR_8888) {
                D32.bits.out_rgb_type = 1;
            } else if (config->pixel_format == PIXEL_FORMAT_ARGB_1555) {
                D32.bits.out_rgb_type = 2;
            } else if (config->pixel_format == PIXEL_FORMAT_ABGR_1555) {
                D32.bits.out_rgb_type = 3;
            } else if (config->pixel_format == PIXEL_FORMAT_RGB_565) {
                D32.bits.out_rgb_type = 4;
            } else if (config->pixel_format == PIXEL_FORMAT_BGR_565) {
                D32.bits.out_rgb_type = 5;
            } else if (config->pixel_format == PIXEL_FORMAT_RGB_888) {
                D32.bits.out_rgb_type = 6;
            } else if (config->pixel_format == PIXEL_FORMAT_BGR_888) {
                D32.bits.out_rgb_type = 7;
            }
        }

        jpegd_reg->OUT_TYPE.u32 = D32.u32;
    }

    /*  . JPGD_ALPHA */
    if (config->out_yuv == HI_FALSE) {
        U_JPGD_ALPHA D32;

        D32.u32 = 0;

        D32.bits.jpgd_a = config->alpha;
        jpegd_reg->JPGD_ALPHA.u32 = D32.u32;
    }

    {
        unsigned int i;

        for (i = 0; i < JPEGD_IP_NUM; i++) {
            jpegd_reg->MTN_ADDRESS0_LSB = get_low_addr(config->phy_emar_buffer0);
            jpegd_reg->MTN_ADDRESS0_MSB.u32 = get_high_addr(config->phy_emar_buffer0);

            jpegd_reg->MTN_ADDRESS1_LSB = get_low_addr(config->phy_emar_buffer1);
            jpegd_reg->MTN_ADDRESS1_MSB.u32 = get_high_addr(config->phy_emar_buffer1);
        }
    }

    /*  PICCUT_STARTPOS     U_PICCUT_ENDPOS */
    if (config->out_yuv == HI_FALSE) {
        {
            U_PICCUT_STARTPOS D32;

            D32.u32 = 0;

            D32.bits.jcfg2jytr_pic_startx = 0;
            D32.bits.jcfg2jytr_pic_starty = 0;
            jpegd_reg->PICCUT_STARTPOS.u32 = D32.u32;
        }

        {
            U_PICCUT_ENDPOS D32;

            D32.u32 = 0;

            D32.bits.jcfg2jytr_pic_endx = config->width - 1;
            D32.bits.jcfg2jytr_pic_endy = config->height - 1;
            jpegd_reg->PICCUT_ENDPOS.u32 = D32.u32;
        }
    }

    {
        U_SAMPLING_FACTOR D32;

        D32.u32 = 0;

        D32.bits.v_fac = config->v_fac;
        D32.bits.u_fac = config->u_fac;
        D32.bits.y_fac = config->y_fac;

        jpegd_reg->SAMPLING_FACTOR.u32 = D32.u32;
    }

    /*  13. DRI */
    {
        U_DRI D32;

        D32.u32 = 0;

        D32.bits.jcfg2jvld_ri = config->dri;

        jpegd_reg->DRI.u32 = D32.u32;
    }

    {
        unsigned int i;
        for (i = 0; i < QUANT_TABLE_SIZE; i++) {
            jpegd_reg->QUANT_TABLE[i].u32 = config->quant_table[i];
        }
    }

    {
        unsigned int i;
        for (i = 0; i < HDC_TABLE_SIZE; i++) {
            jpegd_reg->HDC_TABLE[i].u32 = config->huffman_table[i];
        }
    }

    {
        unsigned int i;
        for (i = 0; i < HAC_MIN_TABLE_SIZE; i++) {
            jpegd_reg->HAC_MIN_TABLE[i].u32 = config->huffman_min_table[i];
        }
    }

    {
        unsigned int i;
        for (i = 0; i < HAC_BASE_TABLE_SIZE; i++) {
            jpegd_reg->HAC_BASE_TABLE[i].u32 = config->huffman_base_table[i];
        }
    }

    {
        unsigned int i;
        for (i = 0; i < HAC_SYMBOL_TABLE_SIZE; i++) {
            jpegd_reg->HAC_SYMBOL_TABLE[i] = config->huffman_symbol_table[i];
        }
    }

    return;
}

void jpegd_drv_read_regs(void *reg_base, jpegd_vpu_status *jpegd_vpu_status)
{
    S_JPGD_REGS_TYPE *jpegd_reg = (S_JPGD_REGS_TYPE *)reg_base;

    jpegd_vpu_status->int_dec_finish = jpegd_reg->JPEG_INT.bits.int_dec_fnsh;
    jpegd_vpu_status->int_dec_err = jpegd_reg->JPEG_INT.bits.int_dec_err;
    jpegd_vpu_status->int_over_time = jpegd_reg->JPEG_INT.bits.int_over_time;
    jpegd_vpu_status->int_bs_res = jpegd_reg->JPEG_INT.bits.int_bs_res;
    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


