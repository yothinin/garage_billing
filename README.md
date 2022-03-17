# garage_billing
ระบบออกบิลสำหรับร้านซ่อมรถขนาดเล็ก เป็นตัวอย่างการพัฒนาระบบด้วยโปรแกรมภาษา C โดยใช้ Gtk+ เวอร์ชัน 2.0 ระบบงานจะเป็นระบบง่ายๆ ไม่มีอะไรมากเพียงแค่บันทึกข้อมูลลูกค้าที่นำรถมาซ่อม และลงข้อมูลสต็อคสินค้า(เพื่อค้นหาชื่อสินค้า) แล้วแสดงบนโปรแกรม แต่เจ้าของร้านซ่อมต้องการที่จะป้อนราคาด้วยตนเอง เพราะไม่มีระบบสต็อค จะเป็นร้านที่ซื้ออะไหล่โดยจ้างวินมอเตอร์ไซค์ไปซื้อมาให้

geany compile:
gcc  -g -Wall -Wunused -c "%f"  -DG_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED  -DGDK_PIXBUF_DISABLE_DEPRECATED  -DGTK_DISABLE_DEPRECATED `pkg-config --libs --cflags gtk+-2.0`

This program purpose to print invoice for car repair, no stock, no price just keep only customer for print.
