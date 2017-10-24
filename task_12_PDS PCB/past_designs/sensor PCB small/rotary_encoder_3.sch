<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="7.7.0">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="DCG_encoders">
<packages>
<package name="AS5045_ROTARY_ENCODER">
<smd name="MAG_INC_N" x="-3.81" y="2.31" dx="1.43" dy="0.46" layer="1"/>
<smd name="MAG_DEC_N" x="-3.81" y="1.66" dx="1.43" dy="0.46" layer="1"/>
<smd name="NC0" x="-3.81" y="1.01" dx="1.43" dy="0.46" layer="1"/>
<smd name="NC1" x="-3.81" y="0.36" dx="1.43" dy="0.46" layer="1"/>
<smd name="NC2" x="-3.81" y="-0.29" dx="1.43" dy="0.46" layer="1"/>
<smd name="MODE" x="-3.81" y="-0.94" dx="1.43" dy="0.46" layer="1"/>
<smd name="VSS" x="-3.81" y="-1.59" dx="1.43" dy="0.46" layer="1"/>
<smd name="PROG_DI" x="-3.81" y="-2.24" dx="1.43" dy="0.46" layer="1"/>
<smd name="DO" x="3.78" y="-2.24" dx="1.43" dy="0.46" layer="1"/>
<smd name="CLK" x="3.78" y="-1.59" dx="1.43" dy="0.46" layer="1"/>
<smd name="CSN" x="3.78" y="-0.94" dx="1.43" dy="0.46" layer="1"/>
<smd name="PWM" x="3.78" y="-0.29" dx="1.43" dy="0.46" layer="1"/>
<smd name="NC4" x="3.78" y="0.36" dx="1.43" dy="0.46" layer="1"/>
<smd name="NC3" x="3.78" y="1.01" dx="1.43" dy="0.46" layer="1"/>
<smd name="VDD3V3" x="3.78" y="1.66" dx="1.43" dy="0.46" layer="1"/>
<smd name="VDD5V" x="3.78" y="2.31" dx="1.43" dy="0.46" layer="1"/>
<wire x1="-3.51" y1="2.56" x2="-3.51" y2="3.16" width="0.05" layer="21"/>
<wire x1="-3.51" y1="3.16" x2="3.49" y2="3.16" width="0.05" layer="21"/>
<wire x1="3.49" y1="3.16" x2="3.49" y2="2.56" width="0.05" layer="21"/>
<wire x1="-3.51" y1="-2.54" x2="-3.51" y2="-3.14" width="0.05" layer="21"/>
<wire x1="-3.51" y1="-3.14" x2="3.49" y2="-3.14" width="0.05" layer="21"/>
<wire x1="3.49" y1="-3.14" x2="3.49" y2="-2.54" width="0.05" layer="21"/>
<circle x="-2.21" y="2.56" radius="0.316225" width="0.127" layer="21"/>
</package>
</packages>
<symbols>
<symbol name="AS5045_ROTARY_ENCODER">
<wire x1="0" y1="0" x2="0" y2="45.72" width="0.254" layer="94"/>
<wire x1="0" y1="45.72" x2="30.48" y2="45.72" width="0.254" layer="94"/>
<wire x1="30.48" y1="45.72" x2="30.48" y2="0" width="0.254" layer="94"/>
<wire x1="30.48" y1="0" x2="0" y2="0" width="0.254" layer="94"/>
<pin name="MAG_INC_N" x="-5.08" y="40.64" length="middle"/>
<pin name="MAG_DEC_N" x="-5.08" y="35.56" length="middle"/>
<pin name="NC0" x="-5.08" y="30.48" length="middle"/>
<pin name="NC1" x="-5.08" y="25.4" length="middle"/>
<pin name="NC2" x="-5.08" y="20.32" length="middle"/>
<pin name="MODE" x="-5.08" y="15.24" length="middle"/>
<pin name="VSS" x="-5.08" y="10.16" length="middle"/>
<pin name="PROG_DI" x="-5.08" y="5.08" length="middle"/>
<pin name="DO" x="35.56" y="5.08" length="middle" rot="R180"/>
<pin name="CLK" x="35.56" y="10.16" length="middle" rot="R180"/>
<pin name="CSN" x="35.56" y="15.24" length="middle" rot="R180"/>
<pin name="PWM" x="35.56" y="20.32" length="middle" rot="R180"/>
<pin name="NC4" x="35.56" y="25.4" length="middle" rot="R180"/>
<pin name="NC3" x="35.56" y="30.48" length="middle" rot="R180"/>
<pin name="VDD_3V3" x="35.56" y="35.56" length="middle" rot="R180"/>
<pin name="VDD_5V" x="35.56" y="40.64" length="middle" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="AS5045_ROTARY_ENCODER">
<gates>
<gate name="G$1" symbol="AS5045_ROTARY_ENCODER" x="0" y="0"/>
</gates>
<devices>
<device name="" package="AS5045_ROTARY_ENCODER">
<connects>
<connect gate="G$1" pin="CLK" pad="CLK"/>
<connect gate="G$1" pin="CSN" pad="CSN"/>
<connect gate="G$1" pin="DO" pad="DO"/>
<connect gate="G$1" pin="MAG_DEC_N" pad="MAG_DEC_N"/>
<connect gate="G$1" pin="MAG_INC_N" pad="MAG_INC_N"/>
<connect gate="G$1" pin="MODE" pad="MODE"/>
<connect gate="G$1" pin="NC0" pad="NC0"/>
<connect gate="G$1" pin="NC1" pad="NC1"/>
<connect gate="G$1" pin="NC2" pad="NC2"/>
<connect gate="G$1" pin="NC3" pad="NC3"/>
<connect gate="G$1" pin="NC4" pad="NC4"/>
<connect gate="G$1" pin="PROG_DI" pad="PROG_DI"/>
<connect gate="G$1" pin="PWM" pad="PWM"/>
<connect gate="G$1" pin="VDD_3V3" pad="VDD3V3"/>
<connect gate="G$1" pin="VDD_5V" pad="VDD5V"/>
<connect gate="G$1" pin="VSS" pad="VSS"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="cap_0805">
<packages>
<package name="CAP_0805_100NF">
<smd name="1" x="-0.8128" y="0" dx="0.9144" dy="1.4478" layer="1"/>
<smd name="2" x="0.8128" y="0" dx="0.9144" dy="1.4478" layer="1"/>
<wire x1="-0.3556" y1="-0.7366" x2="-0.3556" y2="0.7366" width="0" layer="51"/>
<wire x1="-0.3556" y1="0.7366" x2="-1.0922" y2="0.7366" width="0" layer="51"/>
<wire x1="-1.0922" y1="-0.7366" x2="-0.3556" y2="-0.7366" width="0" layer="51"/>
<wire x1="0.3556" y1="0.7366" x2="0.3556" y2="-0.7366" width="0" layer="51"/>
<wire x1="0.3556" y1="-0.7366" x2="1.0922" y2="-0.7366" width="0" layer="51"/>
<wire x1="1.0922" y1="0.7366" x2="0.3556" y2="0.7366" width="0" layer="51"/>
<wire x1="-0.3556" y1="-0.7366" x2="0.3556" y2="-0.7366" width="0" layer="51"/>
<wire x1="1.0922" y1="-0.7366" x2="1.0922" y2="0.7366" width="0" layer="51"/>
<wire x1="0.3556" y1="0.7366" x2="-0.3556" y2="0.7366" width="0" layer="51"/>
<wire x1="-1.0922" y1="0.7366" x2="-1.0922" y2="-0.7366" width="0" layer="51"/>
<text x="-3.4544" y="1.27" size="2.0828" layer="25" ratio="10" rot="SR0">&gt;NAME</text>
<text x="-3.4544" y="-3.175" size="2.0828" layer="27" ratio="10" rot="SR0">&gt;VALUE</text>
</package>
</packages>
<symbols>
<symbol name="CAP_0805_100NF_H">
<pin name="C2" x="7.62" y="0" visible="off" length="short" direction="pas" swaplevel="1" rot="R180"/>
<pin name="C1" x="0" y="0" visible="off" length="short" direction="pas" swaplevel="1"/>
<wire x1="3.4798" y1="-1.905" x2="3.4798" y2="0" width="0.2032" layer="94"/>
<wire x1="3.4798" y1="0" x2="3.4798" y2="1.905" width="0.2032" layer="94"/>
<wire x1="4.1148" y1="-1.905" x2="4.1148" y2="0" width="0.2032" layer="94"/>
<wire x1="4.1148" y1="0" x2="4.1148" y2="1.905" width="0.2032" layer="94"/>
<wire x1="4.1148" y1="0" x2="5.08" y2="0" width="0.2032" layer="94"/>
<wire x1="2.54" y1="0" x2="3.4798" y2="0" width="0.2032" layer="94"/>
<text x="-5.6642" y="-9.3472" size="3.4798" layer="96" ratio="10" rot="SR0">&gt;VALUE</text>
<text x="-0.9144" y="2.7686" size="2.0828" layer="95" ratio="10" rot="SR0">&gt;NAME</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="CAP_0805_10UF" prefix="C">
<description>CAPACITOR, 0805</description>
<gates>
<gate name="A" symbol="CAP_0805_100NF_H" x="0" y="0" swaplevel="1"/>
</gates>
<devices>
<device name="" package="CAP_0805_100NF">
<connects>
<connect gate="A" pin="C1" pad="1"/>
<connect gate="A" pin="C2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MPN" value="0805F106Z100CT" constant="no"/>
<attribute name="OC_FARNELL" value="1759421" constant="no"/>
<attribute name="OC_NEWARK" value="" constant="no"/>
<attribute name="SUPPLIER" value="Multicomp" constant="no"/>
<attribute name="TOLERANCE" value="-20/+80%" constant="no"/>
<attribute name="VALUE" value="10uF" constant="no"/>
<attribute name="VOLTAGERATING" value="10V" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="CAP_0805_100NF" prefix="C">
<description>CAPACITOR, 0805</description>
<gates>
<gate name="A" symbol="CAP_0805_100NF_H" x="0" y="0" swaplevel="1"/>
</gates>
<devices>
<device name="" package="CAP_0805_100NF">
<connects>
<connect gate="A" pin="C1" pad="1"/>
<connect gate="A" pin="C2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MPN" value="U0805F104ZCT" constant="no"/>
<attribute name="OC_FARNELL" value="9406409" constant="no"/>
<attribute name="OC_NEWARK" value="38K9281" constant="no"/>
<attribute name="SUPPLIER" value="Multicomp" constant="no"/>
<attribute name="TOLERANCE" value="-20/+80%" constant="no"/>
<attribute name="VALUE" value="100nF" constant="no"/>
<attribute name="VOLTAGERATING" value="50V" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="wirepad">
<description>&lt;b&gt;Single Pads&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="1,6/0,8">
<description>&lt;b&gt;THROUGH-HOLE PAD&lt;/b&gt;</description>
<wire x1="-0.762" y1="0.762" x2="-0.508" y2="0.762" width="0.1524" layer="21"/>
<wire x1="-0.762" y1="0.762" x2="-0.762" y2="0.508" width="0.1524" layer="21"/>
<wire x1="0.762" y1="0.762" x2="0.762" y2="0.508" width="0.1524" layer="21"/>
<wire x1="0.762" y1="0.762" x2="0.508" y2="0.762" width="0.1524" layer="21"/>
<wire x1="0.762" y1="-0.508" x2="0.762" y2="-0.762" width="0.1524" layer="21"/>
<wire x1="0.762" y1="-0.762" x2="0.508" y2="-0.762" width="0.1524" layer="21"/>
<wire x1="-0.508" y1="-0.762" x2="-0.762" y2="-0.762" width="0.1524" layer="21"/>
<wire x1="-0.762" y1="-0.762" x2="-0.762" y2="-0.508" width="0.1524" layer="21"/>
<circle x="0" y="0" radius="0.635" width="0.1524" layer="51"/>
<pad name="1" x="0" y="0" drill="0.8128" diameter="1.6002" shape="octagon"/>
<text x="-0.762" y="1.016" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="0" y="0.6" size="0.0254" layer="27">&gt;VALUE</text>
</package>
</packages>
<symbols>
<symbol name="PAD">
<wire x1="-1.016" y1="1.016" x2="1.016" y2="-1.016" width="0.254" layer="94"/>
<wire x1="-1.016" y1="-1.016" x2="1.016" y2="1.016" width="0.254" layer="94"/>
<text x="-1.143" y="1.8542" size="1.778" layer="95">&gt;NAME</text>
<text x="-1.143" y="-3.302" size="1.778" layer="96">&gt;VALUE</text>
<pin name="P" x="2.54" y="0" visible="off" length="short" direction="pas" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="1,6/0,8" prefix="PAD" uservalue="yes">
<description>&lt;b&gt;THROUGH-HOLE PAD&lt;/b&gt;</description>
<gates>
<gate name="P" symbol="PAD" x="0" y="0"/>
</gates>
<devices>
<device name="" package="1,6/0,8">
<connects>
<connect gate="P" pin="P" pad="1"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="U$1" library="DCG_encoders" deviceset="AS5045_ROTARY_ENCODER" device=""/>
<part name="C1" library="cap_0805" deviceset="CAP_0805_10UF" device="" value="10uF"/>
<part name="C2" library="cap_0805" deviceset="CAP_0805_100NF" device="" value="100nF"/>
<part name="CSN" library="wirepad" deviceset="1,6/0,8" device=""/>
<part name="CLK" library="wirepad" deviceset="1,6/0,8" device=""/>
<part name="5V" library="wirepad" deviceset="1,6/0,8" device=""/>
<part name="DO" library="wirepad" deviceset="1,6/0,8" device=""/>
<part name="GND" library="wirepad" deviceset="1,6/0,8" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="U$1" gate="G$1" x="33.02" y="7.62"/>
<instance part="C1" gate="A" x="76.2" y="43.18" smashed="yes">
<attribute name="VALUE" x="83.2358" y="41.4528" size="1.27" layer="96" ratio="10" rot="SR0"/>
<attribute name="NAME" x="75.2856" y="40.8686" size="2.0828" layer="95" ratio="10" rot="SR0"/>
</instance>
<instance part="C2" gate="A" x="81.28" y="48.26" smashed="yes" rot="R90">
<attribute name="VALUE" x="77.9272" y="52.7558" size="1.27" layer="96" ratio="10" rot="SR90"/>
<attribute name="NAME" x="78.5114" y="47.3456" size="2.0828" layer="95" ratio="10" rot="SR90"/>
</instance>
<instance part="CSN" gate="P" x="88.9" y="22.86" rot="R180"/>
<instance part="CLK" gate="P" x="88.9" y="17.78" rot="R180"/>
<instance part="5V" gate="P" x="96.52" y="48.26" rot="R180"/>
<instance part="DO" gate="P" x="88.9" y="12.7" rot="R180"/>
<instance part="GND" gate="P" x="119.38" y="25.4" rot="R180"/>
</instances>
<busses>
</busses>
<nets>
<net name="5V" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="VDD_5V"/>
<wire x1="68.58" y1="48.26" x2="81.28" y2="48.26" width="0.1524" layer="91"/>
<pinref part="C2" gate="A" pin="C1"/>
<junction x="81.28" y="48.26"/>
<pinref part="5V" gate="P" pin="P"/>
<wire x1="81.28" y1="48.26" x2="93.98" y2="48.26" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="VDD_3V3"/>
<pinref part="C1" gate="A" pin="C1"/>
<wire x1="68.58" y1="43.18" x2="76.2" y2="43.18" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="NC3"/>
<wire x1="68.58" y1="38.1" x2="76.2" y2="38.1" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="NC4"/>
<wire x1="68.58" y1="33.02" x2="76.2" y2="33.02" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="PWM"/>
<wire x1="68.58" y1="27.94" x2="76.2" y2="27.94" width="0.1524" layer="91"/>
</segment>
</net>
<net name="CSN" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="CSN"/>
<pinref part="CSN" gate="P" pin="P"/>
<wire x1="68.58" y1="22.86" x2="86.36" y2="22.86" width="0.1524" layer="91"/>
</segment>
</net>
<net name="CLK" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="CLK"/>
<pinref part="CLK" gate="P" pin="P"/>
<wire x1="68.58" y1="17.78" x2="86.36" y2="17.78" width="0.1524" layer="91"/>
</segment>
</net>
<net name="DO" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="DO"/>
<pinref part="DO" gate="P" pin="P"/>
<wire x1="68.58" y1="12.7" x2="86.36" y2="12.7" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$9" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="MAG_INC_N"/>
<wire x1="27.94" y1="48.26" x2="17.78" y2="48.26" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$10" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="MAG_DEC_N"/>
<wire x1="27.94" y1="43.18" x2="17.78" y2="43.18" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$11" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="NC0"/>
<wire x1="27.94" y1="38.1" x2="17.78" y2="38.1" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$12" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="NC1"/>
<wire x1="27.94" y1="33.02" x2="17.78" y2="33.02" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$13" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="NC2"/>
<wire x1="27.94" y1="27.94" x2="17.78" y2="27.94" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$14" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="MODE"/>
<wire x1="27.94" y1="22.86" x2="17.78" y2="22.86" width="0.1524" layer="91"/>
</segment>
</net>
<net name="GND" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="VSS"/>
<wire x1="27.94" y1="17.78" x2="17.78" y2="17.78" width="0.1524" layer="91"/>
<label x="17.78" y="17.78" size="1.778" layer="95" rot="R180" xref="yes"/>
</segment>
<segment>
<pinref part="C2" gate="A" pin="C2"/>
<wire x1="81.28" y1="55.88" x2="81.28" y2="58.42" width="0.1524" layer="91"/>
<label x="81.28" y="58.42" size="1.778" layer="95" rot="R90" xref="yes"/>
</segment>
<segment>
<pinref part="C1" gate="A" pin="C2"/>
<wire x1="83.82" y1="43.18" x2="88.9" y2="43.18" width="0.1524" layer="91"/>
<label x="88.9" y="43.18" size="1.778" layer="95" xref="yes"/>
</segment>
<segment>
<pinref part="GND" gate="P" pin="P"/>
<label x="111.76" y="25.4" size="1.778" layer="95" rot="R180" xref="yes"/>
<wire x1="116.84" y1="25.4" x2="111.76" y2="25.4" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$16" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="PROG_DI"/>
<wire x1="27.94" y1="12.7" x2="17.78" y2="12.7" width="0.1524" layer="91"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
