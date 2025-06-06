/*
 * clk-dglnt-dynclk.c - Digilent AXI Dynamic Clock (axi_dynclk) Driver
 *
 * Copyright (C) 2015 Digilent
 * Author: Sam Bobrowicz <sbobrowicz@digilentinc.com>
 *
 * Reused code from clk-axi-clkgen.c, Copyright (C) 2012-2013 Analog Devices Inc.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

 #include <linux/platform_device.h>
 #include <linux/clk-provider.h>
 #include <linux/clk.h>
 #include <linux/slab.h>
 #include <linux/io.h>
 #include <linux/of.h>
 #include <linux/module.h>
 #include <linux/err.h>
 #include <linux/kernel.h>
 
 #define CLK_BIT_WEDGE 13
 #define CLK_BIT_NOCOUNT 12
 
 #define ERR_CLKCOUNTCALC 0xFFFFFFFF //This value is used to signal an error
 #define ERR_CLKDIVIDER (1 << CLK_BIT_WEDGE | 1 << CLK_BIT_NOCOUNT)
 
 #define DYNCLK_DIV_1_REGMASK 0x1041
 #define DYNCLK_DEFAULT_FREQ 125000 //25 MHz (125 KHz / 5)
 
 #define MMCM_FREQ_VCOMIN 600000
 #define MMCM_FREQ_VCOMAX 1200000
 #define MMCM_FREQ_PFDMIN 10000
 #define MMCM_FREQ_PFDMAX 450000
 #define MMCM_FREQ_OUTMIN 4000
 #define MMCM_FREQ_OUTMAX 800000
 #define MMCM_DIV_MAX 106
 #define MMCM_FB_MIN 2
 #define MMCM_FB_MAX 64
 #define MMCM_CLKDIV_MAX 128
 #define MMCM_CLKDIV_MIN 1
 
 #define OFST_DISPLAY_CTRL 0x0
 #define OFST_DISPLAY_STATUS 0x4
 #define OFST_DISPLAY_CLK_L 0x8
 #define OFST_DISPLAY_FB_L 0x0C
 #define OFST_DISPLAY_FB_H_CLK_H 0x10
 #define OFST_DISPLAY_DIV 0x14
 #define OFST_DISPLAY_LOCK_L 0x18
 #define OFST_DISPLAY_FLTR_LOCK_H 0x1C
 
 static const u64 lock_lookup[64] = {
	0b0011000110111110100011111010010000000001,
	0b0011000110111110100011111010010000000001,
	0b0100001000111110100011111010010000000001,
	0b0101101011111110100011111010010000000001,
	0b0111001110111110100011111010010000000001,
	0b1000110001111110100011111010010000000001,
	0b1001110011111110100011111010010000000001,
	0b1011010110111110100011111010010000000001,
	0b1100111001111110100011111010010000000001,
	0b1110011100111110100011111010010000000001,
	0b1111111111111000010011111010010000000001,
	0b1111111111110011100111111010010000000001,
	0b1111111111101110111011111010010000000001,
	0b1111111111101011110011111010010000000001,
	0b1111111111101000101011111010010000000001,
	0b1111111111100111000111111010010000000001,
	0b1111111111100011111111111010010000000001,
	0b1111111111100010011011111010010000000001,
	0b1111111111100000110111111010010000000001,
	0b1111111111011111010011111010010000000001,
	0b1111111111011101101111111010010000000001,
	0b1111111111011100001011111010010000000001,
	0b1111111111011010100111111010010000000001,
	0b1111111111011001000011111010010000000001,
	0b1111111111011001000011111010010000000001,
	0b1111111111010111011111111010010000000001,
	0b1111111111010101111011111010010000000001,
	0b1111111111010101111011111010010000000001,
	0b1111111111010100010111111010010000000001,
	0b1111111111010100010111111010010000000001,
	0b1111111111010010110011111010010000000001,
	0b1111111111010010110011111010010000000001,
	0b1111111111010010110011111010010000000001,
	0b1111111111010001001111111010010000000001,
	0b1111111111010001001111111010010000000001,
	0b1111111111010001001111111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001,
	0b1111111111001111101011111010010000000001
 };
 
 static const u32 filter_lookup_low[64] = {
	  0b0001011111,
	  0b0001010111,
	  0b0001111011,
	  0b0001011011,
	  0b0001101011,
	  0b0001110011,
	  0b0001110011,
	  0b0001110011,
	  0b0001110011,
	  0b0001001011,
	  0b0001001011,
	  0b0001001011,
	  0b0010110011,
	  0b0001010011,
	  0b0001010011,
	  0b0001010011,
	  0b0001010011,
	  0b0001010011,
	  0b0001010011,
	  0b0001010011,
	  0b0001010011,
	  0b0001010011,
	  0b0001010011,
	  0b0001100011,
	  0b0001100011,
	  0b0001100011,
	  0b0001100011,
	  0b0001100011,
	  0b0001100011,
	  0b0001100011,
	  0b0001100011,
	  0b0001100011,
	  0b0001100011,
	  0b0001100011,
	  0b0001100011,
	  0b0001100011,
	  0b0001100011,
	  0b0010010011,
	  0b0010010011,
	  0b0010010011,
	  0b0010010011,
	  0b0010010011,
	  0b0010010011,
	  0b0010010011,
	  0b0010010011,
	  0b0010010011,
	  0b0010010011,
	  0b0010100011,
	  0b0010100011,
	  0b0010100011,
	  0b0010100011,
	  0b0010100011,
	  0b0010100011,
	  0b0010100011,
	  0b0010100011,
	  0b0010100011,
	  0b0010100011,
	  0b0010100011,
	  0b0010100011,
	  0b0010100011,
	  0b0010100011,
	  0b0010100011,
	  0b0010100011,
	  0b0010100011
 };
 
 struct dglnt_dynclk_reg;
 struct dglnt_dynclk_mode;
 struct dglnt_dynclk;
 
 struct dglnt_dynclk_reg{
		 u32 clk0L;
		 u32 clkFBL;
		 u32 clkFBH_clk0H;
		 u32 divclk;
		 u32 lockL;
		 u32 fltr_lockH;
 };
 
 struct dglnt_dynclk_mode{
		 u32 freq;
		 u32 fbmult;
		 u32 clkdiv;
		 u32 maindiv;
 };
 
 struct dglnt_dynclk {
	 void __iomem *base;
	 struct clk_hw clk_hw;
	unsigned long freq;
 };
 
 u32 dglnt_dynclk_divider(u32 divide)
 {
	 u32 output = 0;
	 u32 highTime = 0;
	 u32 lowTime = 0;
 
	 if ((divide < 1) || (divide > 128))
		 return ERR_CLKDIVIDER;
 
	 if (divide == 1)
		 return DYNCLK_DIV_1_REGMASK;
 
	 highTime = divide / 2;
	 if (divide & 0b1) //if divide is odd
	 {
		 lowTime = highTime + 1;
		 output = 1 << CLK_BIT_WEDGE;
	 }
	 else
	 {
		 lowTime = highTime;
	 }
 
	 output |= 0x03F & lowTime;
	 output |= 0xFC0 & (highTime << 6);
	 return output;
 }
 
 u32 dglnt_dynclk_count_calc(u32 divide)
 {
	 u32 output = 0;
	 u32 divCalc = 0;
 
	 divCalc = dglnt_dynclk_divider(divide);
	 if (divCalc == ERR_CLKDIVIDER)
		 output = ERR_CLKCOUNTCALC;
	 else
		 output = (0xFFF & divCalc) | ((divCalc << 10) & 0x00C00000);
	 return output;
 }
 
 
 int dglnt_dynclk_find_reg (struct dglnt_dynclk_reg *regValues, struct dglnt_dynclk_mode *clkParams)
 {
	 if ((clkParams->fbmult < 2) || clkParams->fbmult > 64 )
		 return -EINVAL;
 
	 regValues->clk0L = dglnt_dynclk_count_calc(clkParams->clkdiv);
	 if (regValues->clk0L == ERR_CLKCOUNTCALC)
		 return -EINVAL;
 
	 regValues->clkFBL = dglnt_dynclk_count_calc(clkParams->fbmult);
	 if (regValues->clkFBL == ERR_CLKCOUNTCALC)
		 return -EINVAL;
 
	 regValues->clkFBH_clk0H = 0;
 
	 regValues->divclk = dglnt_dynclk_divider(clkParams->maindiv);
	 if (regValues->divclk == ERR_CLKDIVIDER)
		 return -EINVAL;
 
	 regValues->lockL = (u32) (lock_lookup[clkParams->fbmult - 1] & 0xFFFFFFFF);
 
	 regValues->fltr_lockH = (u32) ((lock_lookup[clkParams->fbmult - 1] >> 32) & 0x000000FF);
	 regValues->fltr_lockH |= ((filter_lookup_low[clkParams->fbmult - 1] << 16) & 0x03FF0000);
 
	 return 0;
 }
 
 void dglnt_dynclk_write_reg (struct dglnt_dynclk_reg *regValues, void __iomem *baseaddr)
 {
	writel(regValues->clk0L, baseaddr + OFST_DISPLAY_CLK_L);
	writel(regValues->clkFBL, baseaddr + OFST_DISPLAY_FB_L);
	writel(regValues->clkFBH_clk0H, baseaddr + OFST_DISPLAY_FB_H_CLK_H);
	writel(regValues->divclk, baseaddr + OFST_DISPLAY_DIV);
	writel(regValues->lockL, baseaddr + OFST_DISPLAY_LOCK_L);
	writel(regValues->fltr_lockH, baseaddr + OFST_DISPLAY_FLTR_LOCK_H);
 }
 
 
 u32 dglnt_dynclk_find_mode(u32 freq, u32 parentFreq, struct dglnt_dynclk_mode *bestPick)
 {
	 u32 bestError = MMCM_FREQ_OUTMAX;
	 u32 curError;
	 u32 curClkMult;
	 u32 curFreq;
	u32 divVal;
	 u32 curFb, curClkDiv;
	 u32 minFb = 0;
	 u32 maxFb = 0;
	u32 curDiv = 1;
	u32 maxDiv;
	bool freq_found = false;
 
	 bestPick->freq = 0;
	if (parentFreq == 0)
	   return 0; 
 
	if (freq < MMCM_FREQ_OUTMIN)//minimum frequency is actually dictated by VCOmin
	   freq = MMCM_FREQ_OUTMIN;
	if (freq > MMCM_FREQ_OUTMAX)
	   freq = MMCM_FREQ_OUTMAX;
 
	if (parentFreq > MMCM_FREQ_PFDMAX)
	   curDiv = 2;      
	maxDiv = parentFreq / MMCM_FREQ_PFDMIN;
	if (maxDiv > MMCM_DIV_MAX)
	   maxDiv = MMCM_DIV_MAX;
 
	 while (curDiv <= maxDiv && !freq_found)
	 {
		 minFb = curDiv * DIV_ROUND_UP(MMCM_FREQ_VCOMIN, parentFreq); 
		 maxFb = curDiv * (MMCM_FREQ_VCOMAX / parentFreq); 
		 if (maxFb > MMCM_FB_MAX)
			 maxFb = MMCM_FB_MAX;
		 if (minFb < MMCM_FB_MIN)
			 minFb = MMCM_FB_MIN;
 
	   divVal = curDiv * freq;
	   curClkMult = ((parentFreq * 1000) + (divVal / 2)) / divVal; //This multiplier is used to find the best clkDiv value for each FB value
		 
		 curFb = minFb;
		while (curFb <= maxFb && !freq_found)
		{
			curClkDiv = ((curClkMult * curFb) + 500) / 1000;
		  if (curClkDiv > MMCM_CLKDIV_MAX)
			  curClkDiv = MMCM_CLKDIV_MAX;
		  if (curClkDiv < MMCM_CLKDIV_MIN)
			 curClkDiv = MMCM_CLKDIV_MIN;
			curFreq = (((parentFreq * curFb) / curDiv) / curClkDiv);
		  if (curFreq >= freq)			
			 curError = curFreq - freq;
		  else
			 curError = freq - curFreq;
			if (curError < bestError)
			{
				bestError = curError;
				bestPick->clkdiv = curClkDiv;
				bestPick->fbmult = curFb;
				bestPick->maindiv = curDiv;
				bestPick->freq = curFreq;
			}
		  if (!curError)
			 freq_found = true;
			curFb++;
	   }
	   curDiv++;
	 }
	return bestPick->freq;
 }
 
 static struct dglnt_dynclk *clk_hw_to_dglnt_dynclk(struct clk_hw *clk_hw)
 {
	 return container_of(clk_hw, struct dglnt_dynclk, clk_hw);
 }
 
 
 static int dglnt_dynclk_enable(struct clk_hw *clk_hw)
 {
	 struct dglnt_dynclk *dglnt_dynclk = clk_hw_to_dglnt_dynclk(clk_hw);
	 unsigned int clock_state;
 
	 if (dglnt_dynclk->freq)
	 {
		 writel(1, dglnt_dynclk->base + OFST_DISPLAY_CTRL);
		 do
		 {
			 clock_state = readl(dglnt_dynclk->base + OFST_DISPLAY_STATUS);
		 } while (!clock_state);
	 }
	 return 0;
 }
 
 static void dglnt_dynclk_disable(struct clk_hw *clk_hw)
 {
	 struct dglnt_dynclk *dglnt_dynclk = clk_hw_to_dglnt_dynclk(clk_hw);
 
	writel(0, dglnt_dynclk->base + OFST_DISPLAY_CTRL);
 }
 
 static int dglnt_dynclk_set_rate(struct clk_hw *clk_hw,
	 unsigned long rate, unsigned long parent_rate)
 {
	 struct dglnt_dynclk *dglnt_dynclk = clk_hw_to_dglnt_dynclk(clk_hw);
	 struct dglnt_dynclk_reg clkReg;
	 struct dglnt_dynclk_mode clkMode;
 
	 if (parent_rate == 0 || rate == 0)
		 return -EINVAL;
	if (rate == dglnt_dynclk->freq)
	   return 0;
 
	 rate = (rate + 100) / 200; //Convert from Hz to KHz, then multiply by five to account for BUFR division
	 parent_rate = (parent_rate + 500) / 1000; //convert from Hz to KHz 
	if(!dglnt_dynclk_find_mode(rate, parent_rate, &clkMode))
	   return -EINVAL;
 
	 /*
	  * Write to the PLL dynamic configuration registers to configure it with the calculated
	  * parameters.
	  */
	 dglnt_dynclk_find_reg(&clkReg, &clkMode);
	 dglnt_dynclk_write_reg(&clkReg, dglnt_dynclk->base);
	dglnt_dynclk->freq = clkMode.freq * 200;
	dglnt_dynclk_disable(clk_hw);
	dglnt_dynclk_enable(clk_hw);
	 return 0;
 }
 
 static long dglnt_dynclk_round_rate(struct clk_hw *hw, unsigned long rate,
	 unsigned long *parent_rate)
 {
	 struct dglnt_dynclk_mode clkMode;
	
	dglnt_dynclk_find_mode(((rate + 100) / 200), ((*parent_rate) + 500) / 1000, &clkMode);
 
	 return (clkMode.freq * 200);
 }
 
 static unsigned long dglnt_dynclk_recalc_rate(struct clk_hw *clk_hw,
	 unsigned long parent_rate)
 {
	 struct dglnt_dynclk *dglnt_dynclk = clk_hw_to_dglnt_dynclk(clk_hw);
	 
	 return dglnt_dynclk->freq;
 }
 
 
 static const struct clk_ops dglnt_dynclk_ops = {
	 .recalc_rate = dglnt_dynclk_recalc_rate,
	 .round_rate = dglnt_dynclk_round_rate,
	 .set_rate = dglnt_dynclk_set_rate,
	 .enable = dglnt_dynclk_enable,
	 .disable = dglnt_dynclk_disable,
 };
 
 static const struct of_device_id dglnt_dynclk_ids[] = {
	 { .compatible = "dglnt,axi-dynclk",}, 
	 { },
 };
 MODULE_DEVICE_TABLE(of, dglnt_dynclk_ids);
 
 static int dglnt_dynclk_probe(struct platform_device *pdev)
 {
	 const struct of_device_id *id;
	 struct dglnt_dynclk *dglnt_dynclk;
	 struct clk_init_data init;
	 const char *parent_name;
	 const char *clk_name;
	 struct resource *mem;
	 struct clk *clk;
 
	 if (!pdev->dev.of_node)
		 return -ENODEV;
 
	 id = of_match_node(dglnt_dynclk_ids, pdev->dev.of_node);
	 if (!id)
		 return -ENODEV;
 
	 dglnt_dynclk = devm_kzalloc(&pdev->dev, sizeof(*dglnt_dynclk), GFP_KERNEL);
	 if (!dglnt_dynclk)
		 return -ENOMEM;
 
	 mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	 dglnt_dynclk->base = devm_ioremap_resource(&pdev->dev, mem);
	 if (IS_ERR(dglnt_dynclk->base))
		 return PTR_ERR(dglnt_dynclk->base);
 
	 parent_name = of_clk_get_parent_name(pdev->dev.of_node, 0);
	 if (!parent_name)
		 return -EINVAL;
 
	 clk_name = pdev->dev.of_node->name;
	 of_property_read_string(pdev->dev.of_node, "clock-output-names",
		 &clk_name);
 
	 init.name = clk_name;
	 init.ops = &dglnt_dynclk_ops;
	 init.flags = 0;
	 init.parent_names = &parent_name;
	 init.num_parents = 1;
	
	dglnt_dynclk->freq = 0;
	 dglnt_dynclk_disable(&dglnt_dynclk->clk_hw);
 
	 dglnt_dynclk->clk_hw.init = &init;
	 clk = devm_clk_register(&pdev->dev, &dglnt_dynclk->clk_hw);
	 if (IS_ERR(clk))
		 return PTR_ERR(clk);
 
	 return of_clk_add_provider(pdev->dev.of_node, of_clk_src_simple_get,
					 clk);
 }
 
 static int dglnt_dynclk_remove(struct platform_device *pdev)
 {
	 of_clk_del_provider(pdev->dev.of_node);
 
	 return 0;
 }
 
 static struct platform_driver dglnt_dynclk_driver = {
	 .driver = {
		 .name = "dglnt-dynclk",
		 .owner = THIS_MODULE,
		 .of_match_table = dglnt_dynclk_ids,
	 },
	 .probe = dglnt_dynclk_probe,
	 .remove = dglnt_dynclk_remove,
 };
 module_platform_driver(dglnt_dynclk_driver);
 
 MODULE_LICENSE("GPL v2");
 MODULE_AUTHOR("Sam Bobrowicz <sbobrowicz@digilentinc.com>");
 MODULE_DESCRIPTION("CCF Driver for Digilent axi_dynclk IP Core");
 