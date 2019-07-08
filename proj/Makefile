PROG=proj

.PATH: ${.CURDIR}/src

SRCS += sllist.c player.c keyboard.c video_card.c sprite.c timer.c mouse.c game_setup.c int_handlers.c rtc.c enemies.c cursor.c utils.c hud.c

SRCS += proj.c

CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
