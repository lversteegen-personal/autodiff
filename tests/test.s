	.file	"test.cpp"
 # GNU C++17 (MinGW-W64 x86_64-msvcrt-posix-seh, built by Brecht Sanders) version 13.1.0 (x86_64-w64-mingw32)
 #	compiled by GNU C version 13.1.0, GMP version 6.2.1, MPFR version 4.2.0, MPC version 1.3.1, isl version isl-0.26-GMP

 # GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
 # options passed: -mtune=generic -march=x86-64 -O2
	.text
	.section	.text$_ZNKSt5ctypeIcE8do_widenEc,"x"
	.linkonce discard
	.align 2
	.p2align 4
	.globl	_ZNKSt5ctypeIcE8do_widenEc
	.def	_ZNKSt5ctypeIcE8do_widenEc;	.scl	2;	.type	32;	.endef
	.seh_proc	_ZNKSt5ctypeIcE8do_widenEc
_ZNKSt5ctypeIcE8do_widenEc:
.LFB1842:
	.seh_endprologue
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/locale_facets.h:1092:       do_widen(char __c) const
	movl	%edx, %eax	 # tmp87, __c
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/locale_facets.h:1093:       { return __c; }
	ret	
	.seh_endproc
	.text
	.p2align 4
	.def	_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_.isra.0;	.scl	3;	.type	32;	.endef
	.seh_proc	_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_.isra.0
_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_.isra.0:
.LFB4489:
	pushq	%rsi	 #
	.seh_pushreg	%rsi
	pushq	%rbx	 #
	.seh_pushreg	%rbx
	subq	$40, %rsp	 #,
	.seh_stackalloc	40
	.seh_endprologue
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:736:     { return flush(__os.put(__os.widen('\n'))); }
	movq	(%rcx), %rax	 # __os_1(D)->_vptr.basic_ostream, __os_1(D)->_vptr.basic_ostream
	movq	-24(%rax), %rax	 # MEM[(long long int *)_2 + -24B], MEM[(long long int *)_2 + -24B]
	movq	240(%rcx,%rax), %rsi	 # MEM[(const struct __ctype_type * *)_5 + 240B], _14
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:735:     endl(basic_ostream<_CharT, _Traits>& __os)
	movq	%rcx, %rbx	 # tmp101, __os
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_ios.h:49:       if (!__f)
	testq	%rsi, %rsi	 # _14
	je	.L8	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/locale_facets.h:882: 	if (_M_widen_ok)
	cmpb	$0, 56(%rsi)	 #, MEM[(const struct ctype *)_14]._M_widen_ok
	je	.L5	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/locale_facets.h:883: 	  return _M_widen[static_cast<unsigned char>(__c)];
	movsbl	67(%rsi), %edx	 # MEM[(const struct ctype *)_14]._M_widen[10],
.L6:
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:736:     { return flush(__os.put(__os.widen('\n'))); }
	movq	%rbx, %rcx	 # __os,
	call	_ZNSo3putEc	 #
	movq	%rax, %rcx	 # tmp103, _8
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:736:     { return flush(__os.put(__os.widen('\n'))); }
	addq	$40, %rsp	 #,
	popq	%rbx	 #
	popq	%rsi	 #
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:758:     { return __os.flush(); }
	jmp	_ZNSo5flushEv	 #
.L5:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/locale_facets.h:884: 	this->_M_widen_init();
	movq	%rsi, %rcx	 # _14,
	call	_ZNKSt5ctypeIcE13_M_widen_initEv	 #
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/locale_facets.h:885: 	return this->do_widen(__c);
	movq	(%rsi), %rax	 # MEM[(const struct ctype *)_14].D.44691._vptr.facet, MEM[(const struct ctype *)_14].D.44691._vptr.facet
	movl	$10, %edx	 #,
	leaq	_ZNKSt5ctypeIcE8do_widenEc(%rip), %rcx	 #, tmp99
	movq	48(%rax), %rax	 # MEM[(int (*) () *)_27 + 48B], _28
	cmpq	%rcx, %rax	 # tmp99, _28
	je	.L6	 #,
	movl	$10, %edx	 #,
	movq	%rsi, %rcx	 # _14,
	call	*%rax	 # _28
	movsbl	%al, %edx	 # tmp102,
	jmp	.L6	 #
.L8:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_ios.h:50: 	__throw_bad_cast();
	call	_ZSt16__throw_bad_castv	 #
	nop	
	.seh_endproc
	.def	__main;	.scl	2;	.type	32;	.endef
	.section	.text.unlikely,"x"
.LCOLDB0:
	.section	.text.startup,"x"
.LHOTB0:
	.p2align 4
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
.LFB3776:
	pushq	%rdi	 #
	.seh_pushreg	%rdi
	pushq	%rsi	 #
	.seh_pushreg	%rsi
	pushq	%rbx	 #
	.seh_pushreg	%rbx
	subq	$2576, %rsp	 #,
	.seh_stackalloc	2576
	.seh_endprologue
 # test.cpp:14: {
	call	__main	 #
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:200: 	: allocator_type(__a), _M_p(__dat) { }
	leaq	32(%rsp), %rdi	 #, tmp128
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.h:1637:     random_device() { _M_init("default"); }
	leaq	64(%rsp), %rsi	 #, tmp129
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/char_traits.h:445: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movl	$1634100580, 48(%rsp)	 #, MEM <char[1:7]> [(void *)&D.79603 + 16B]
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:200: 	: allocator_type(__a), _M_p(__dat) { }
	leaq	48(%rsp), %rbx	 #, tmp130
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.h:1637:     random_device() { _M_init("default"); }
	movq	%rdi, %rdx	 # tmp128,
	movq	%rsi, %rcx	 # tmp129,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/char_traits.h:368: 	__c1 = __c2;
	movb	$0, 55(%rsp)	 #, MEM[(char_type &)&D.79603 + 23]
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:200: 	: allocator_type(__a), _M_p(__dat) { }
	movq	%rbx, 32(%rsp)	 # tmp130, MEM[(struct _Alloc_hider *)&D.79603]._M_p
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/char_traits.h:445: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movl	$1953264993, 51(%rsp)	 #, MEM <char[1:7]> [(void *)&D.79603 + 16B]
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:229:       { _M_string_length = __length; }
	movq	$7, 40(%rsp)	 #, D.79603._M_string_length
.LEHB0:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.h:1637:     random_device() { _M_init("default"); }
	call	_ZNSt13random_device7_M_initERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE	 #
.LEHE0:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:234:       { return _M_dataplus._M_p; }
	movq	32(%rsp), %rcx	 # D.79603._M_dataplus._M_p, _39
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:275: 	if (_M_data() == _M_local_data())
	cmpq	%rbx, %rcx	 # tmp130, _39
	je	.L10	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:300:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	48(%rsp), %rax	 # D.79603.D.29453._M_allocated_capacity, tmp138
	leaq	1(%rax), %rdx	 #, tmp106
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/new_allocator.h:168: 	_GLIBCXX_OPERATOR_DELETE(_GLIBCXX_SIZED_DEALLOC(__p, __n));
	call	_ZdlPvy	 #
.L10:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.h:1659:     { return this->_M_getval(); }
	movq	%rsi, %rcx	 # tmp129,
.LEHB1:
	call	_ZNSt13random_device9_M_getvalEv	 #
 # test.cpp:16:     int a = randomDevice() % 100;
	movl	%eax, %ebx	 # _9, _9
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.h:1659:     { return this->_M_getval(); }
	movq	%rsi, %rcx	 # tmp129,
 # test.cpp:16:     int a = randomDevice() % 100;
	imulq	$1374389535, %rbx, %rbx	 #, _9, tmp112
	shrq	$37, %rbx	 #, a
	imull	$100, %ebx, %edx	 #, a, tmp114
	subl	%edx, %eax	 # tmp114, _9
	movl	%eax, %ebx	 # _9, a
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.h:1659:     { return this->_M_getval(); }
	call	_ZNSt13random_device9_M_getvalEv	 #
 # test.cpp:17:     int b = randomDevice() % 100;
	movl	%eax, %edx	 # _8, _8
 # test.cpp:5: inline int subtract(int a, int b) { return a - b; }
	movl	%ebx, %edi	 # a, _15
 # test.cpp:17:     int b = randomDevice() % 100;
	imulq	$1374389535, %rdx, %rdx	 #, _8, tmp117
	shrq	$37, %rdx	 #, b
	imull	$100, %edx, %ecx	 #, b, tmp119
	subl	%ecx, %eax	 # tmp119, _8
	movl	%eax, %edx	 # _8, b
 # test.cpp:5: inline int subtract(int a, int b) { return a - b; }
	subl	%eax, %edi	 # b, _15
 # test.cpp:4: inline int add(int a, int b) { return a + b; }
	addl	%ebx, %edx	 # a, tmp120
 # test.cpp:22:     std::cout << s << std::endl;
	movq	.refptr._ZSt4cout(%rip), %rbx	 #, tmp127
	movq	%rbx, %rcx	 # tmp127,
	call	_ZNSolsEi	 #
	movq	%rax, %rcx	 # tmp134, _11
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:115: 	return __pf(*this);
	call	_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_.isra.0	 #
 # test.cpp:23:     std::cout << d << std::endl;
	movl	%edi, %edx	 # _15,
	movq	%rbx, %rcx	 # tmp127,
	call	_ZNSolsEi	 #
	movq	%rax, %rcx	 # tmp135, _13
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:115: 	return __pf(*this);
	call	_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_.isra.0	 #
.LEHE1:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.h:1643:     { _M_fini(); }
	movq	%rsi, %rcx	 # tmp129,
	call	_ZNSt13random_device7_M_finiEv	 #
 # test.cpp:25: }
	xorl	%eax, %eax	 #
	addq	$2576, %rsp	 #,
	popq	%rbx	 #
	popq	%rsi	 #
	popq	%rdi	 #
	ret	
.L15:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:803:       { _M_dispose(); }
	movq	%rax, %rbx	 # tmp132, tmp110
	jmp	.L12	 #
.L14:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.h:1643:     { _M_fini(); }
	movq	%rax, %rbx	 # tmp136, tmp126
	jmp	.L13	 #
	.def	__gxx_personality_seh0;	.scl	2;	.type	32;	.endef
	.seh_handler	__gxx_personality_seh0, @unwind, @except
	.seh_handlerdata
.LLSDA3776:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE3776-.LLSDACSB3776
.LLSDACSB3776:
	.uleb128 .LEHB0-.LFB3776
	.uleb128 .LEHE0-.LEHB0
	.uleb128 .L15-.LFB3776
	.uleb128 0
	.uleb128 .LEHB1-.LFB3776
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L14-.LFB3776
	.uleb128 0
.LLSDACSE3776:
	.section	.text.startup,"x"
	.seh_endproc
	.section	.text.unlikely,"x"
	.def	main.cold;	.scl	3;	.type	32;	.endef
	.seh_proc	main.cold
	.seh_stackalloc	2600
	.seh_savereg	%rbx, 2576
	.seh_savereg	%rsi, 2584
	.seh_savereg	%rdi, 2592
	.seh_endprologue
main.cold:
.L12:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:803:       { _M_dispose(); }
	movq	%rdi, %rcx	 # tmp128,
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv	 #
	movq	%rbx, %rcx	 # tmp110,
.LEHB2:
	call	_Unwind_Resume	 #
.LEHE2:
.L13:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.h:1643:     { _M_fini(); }
	movq	%rsi, %rcx	 # tmp129,
	call	_ZNSt13random_device7_M_finiEv	 #
	movq	%rbx, %rcx	 # tmp126,
.LEHB3:
	call	_Unwind_Resume	 #
	nop	
.LEHE3:
	.seh_handler	__gxx_personality_seh0, @unwind, @except
	.seh_handlerdata
.LLSDAC3776:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSEC3776-.LLSDACSBC3776
.LLSDACSBC3776:
	.uleb128 .LEHB2-.LCOLDB0
	.uleb128 .LEHE2-.LEHB2
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB3-.LCOLDB0
	.uleb128 .LEHE3-.LEHB3
	.uleb128 0
	.uleb128 0
.LLSDACSEC3776:
	.section	.text.unlikely,"x"
	.section	.text.startup,"x"
	.section	.text.unlikely,"x"
	.seh_endproc
.LCOLDE0:
	.section	.text.startup,"x"
.LHOTE0:
	.ident	"GCC: (MinGW-W64 x86_64-msvcrt-posix-seh, built by Brecht Sanders) 13.1.0"
	.def	_ZNSo3putEc;	.scl	2;	.type	32;	.endef
	.def	_ZNSo5flushEv;	.scl	2;	.type	32;	.endef
	.def	_ZNKSt5ctypeIcE13_M_widen_initEv;	.scl	2;	.type	32;	.endef
	.def	_ZSt16__throw_bad_castv;	.scl	2;	.type	32;	.endef
	.def	_ZNSt13random_device7_M_initERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE;	.scl	2;	.type	32;	.endef
	.def	_ZdlPvy;	.scl	2;	.type	32;	.endef
	.def	_ZNSt13random_device9_M_getvalEv;	.scl	2;	.type	32;	.endef
	.def	_ZNSolsEi;	.scl	2;	.type	32;	.endef
	.def	_ZNSt13random_device7_M_finiEv;	.scl	2;	.type	32;	.endef
	.def	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv;	.scl	2;	.type	32;	.endef
	.def	_Unwind_Resume;	.scl	2;	.type	32;	.endef
	.section	.rdata$.refptr._ZSt4cout, "dr"
	.globl	.refptr._ZSt4cout
	.linkonce	discard
.refptr._ZSt4cout:
	.quad	_ZSt4cout
