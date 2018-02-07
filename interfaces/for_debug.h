#ifndef	_FOR_DEBUG_H_
#define	_FOR_DEBUG_H_

#include <stdio.h>

#define var2str(name_31415926) (#name_31415926)
#define	asn_debug_indent	0
#define CITS_DEBUG_INDENT_ADD(i) do{}while(0)
/*
 * A macro for debugging.
 * You may enable or override it.
 */
#ifndef	CITS_DEBUG	/* If debugging code is not defined elsewhere... */
	#if	EMIT_CITS_DEBUG == 1	/* And it was asked to emit this code... */
		#define abc 0
		#ifdef	__GNUC__
			#ifdef	ASN_THREAD_SAFE
			/* Thread safety requires sacrifice in output indentation:
			 * Retain empty definition of CITS_DEBUG_INDENT_ADD. */
			#else	/* !ASN_THREAD_SAFE */
				#undef  CITS_DEBUG_INDENT_ADD
				#undef  asn_debug_indent
				int asn_debug_indent;
				#define CITS_DEBUG_INDENT_ADD(i) do { asn_debug_indent += i; } while(0)
			#endif	/* ASN_THREAD_SAFE */
			#define	CITS_DEBUG(fmt, args...)	do {			\
					int adi = asn_debug_indent;		\
					while(adi--) fprintf(stderr, " ");	\
					fprintf(stderr, fmt, ##args);		\
					fprintf(stderr, " (%s:%d)\n",		\
						__FILE__, __LINE__);		\
				} while(0)
		#else	/* !__GNUC__ */
			void CITS_DEBUG_f(const char *fmt, ...);
			#define	CITS_DEBUG	CITS_DEBUG_f
		#endif	/* __GNUC__ */
	#else	/* EMIT_CITS_DEBUG != 1 */
		static inline void CITS_DEBUG(const char *fmt, ...) { (void)fmt; }
	#endif	/* EMIT_CITS_DEBUG */
#endif	/* CITS_DEBUG */

#endif