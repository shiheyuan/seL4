/*
 * Copyright 2014, General Dynamics C4 Systems
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(GD_GPL)
 */

#include <config.h>
#include <arch/kernel/boot_sys.h>
#include <arch/model/statedata.h>
#include <machine/io.h>
#include <plat/machine/io.h>

#if defined(CONFIG_DEBUG_BUILD) || defined(CONFIG_PRINTING)
void
serial_init(uint16_t port)
{
    while (!(in8(port + 5) & 0x60)); /* wait until not busy */

    out8(port + 1, 0x00); /* disable generating interrupts */
    out8(port + 3, 0x80); /* line control register: command: set divisor */
    out8(port,     0x01); /* set low byte of divisor to 0x01 = 115200 baud */
    out8(port + 1, 0x00); /* set high byte of divisor to 0x00 */
    out8(port + 3, 0x03); /* line control register: set 8 bit, no parity, 1 stop bit */
    out8(port + 4, 0x0b); /* modem control register: set DTR/RTS/OUT2 */

    in8(port);     /* clear recevier port */
    in8(port + 5); /* clear line status port */
    in8(port + 6); /* clear modem status port */
}
#endif /* CONFIG_PRINTING || CONFIG_DEBUG_BUILD */

#ifdef CONFIG_PRINTING
void
putConsoleChar(unsigned char a)
{
    while (x86KSconsolePort && !(in8(x86KSconsolePort + 5) & 0x20));
    out8(x86KSconsolePort, a);
}
#endif /* CONFIG_PRINTING */

#ifdef CONFIG_DEBUG_BUILD
void
putDebugChar(unsigned char a)
{
    while (x86KSdebugPort && (in8(x86KSdebugPort + 5) & 0x20) == 0);
    out8(x86KSdebugPort, a);
}

unsigned char
getDebugChar(void)
{
    while ((in8(x86KSdebugPort + 5) & 1) == 0);
    return in8(x86KSdebugPort);
}
#endif /* CONFIG_DEBUG_BUILD */
