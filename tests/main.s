	.file	"main.cpp"
 # GNU C++17 (MinGW-W64 x86_64-msvcrt-posix-seh, built by Brecht Sanders) version 13.1.0 (x86_64-w64-mingw32)
 #	compiled by GNU C version 13.1.0, GMP version 6.2.1, MPFR version 4.2.0, MPC version 1.3.1, isl version isl-0.26-GMP

 # GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
 # options passed: -mtune=generic -march=x86-64 -O3
	.text
	.section	.text$_ZNSt17_Function_handlerIFdddEZNK5ArrayIdEmlERKS2_EUlddE_E9_M_invokeERKSt9_Any_dataOdSA_,"x"
	.linkonce discard
	.p2align 4
	.globl	_ZNSt17_Function_handlerIFdddEZNK5ArrayIdEmlERKS2_EUlddE_E9_M_invokeERKSt9_Any_dataOdSA_
	.def	_ZNSt17_Function_handlerIFdddEZNK5ArrayIdEmlERKS2_EUlddE_E9_M_invokeERKSt9_Any_dataOdSA_;	.scl	2;	.type	32;	.endef
	.seh_proc	_ZNSt17_Function_handlerIFdddEZNK5ArrayIdEmlERKS2_EUlddE_E9_M_invokeERKSt9_Any_dataOdSA_
_ZNSt17_Function_handlerIFdddEZNK5ArrayIdEmlERKS2_EUlddE_E9_M_invokeERKSt9_Any_dataOdSA_:
.LFB6477:
	.seh_endprologue
 # array.hpp:505:         static auto f = [](T a, T b) { return a * b; };
	movsd	(%rdx), %xmm0	 # *__args#0_4(D), *__args#0_4(D)
	mulsd	(%r8), %xmm0	 # *__args#1_5(D), tmp88
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/std_function.h:292:       }
	ret	
	.seh_endproc
	.section	.text$_ZNSt17_Function_handlerIFdddEZNK5ArrayIdEmlERKS2_EUlddE_E10_M_managerERSt9_Any_dataRKS7_St18_Manager_operation,"x"
	.linkonce discard
	.p2align 4
	.globl	_ZNSt17_Function_handlerIFdddEZNK5ArrayIdEmlERKS2_EUlddE_E10_M_managerERSt9_Any_dataRKS7_St18_Manager_operation
	.def	_ZNSt17_Function_handlerIFdddEZNK5ArrayIdEmlERKS2_EUlddE_E10_M_managerERSt9_Any_dataRKS7_St18_Manager_operation;	.scl	2;	.type	32;	.endef
	.seh_proc	_ZNSt17_Function_handlerIFdddEZNK5ArrayIdEmlERKS2_EUlddE_E10_M_managerERSt9_Any_dataRKS7_St18_Manager_operation
_ZNSt17_Function_handlerIFdddEZNK5ArrayIdEmlERKS2_EUlddE_E10_M_managerERSt9_Any_dataRKS7_St18_Manager_operation:
.LFB6481:
	.seh_endprologue
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/std_function.h:270: 	switch (__op)
	testl	%r8d, %r8d	 # __op
	je	.L4	 #,
	cmpl	$1, %r8d	 #, __op
	je	.L5	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/std_function.h:285:       }
	xorl	%eax, %eax	 #
	ret	
	.p2align 4,,10
	.p2align 3
.L5:
	xorl	%eax, %eax	 #
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/std_function.h:278: 	    __dest._M_access<_Functor*>() = _Base::_M_get_pointer(__source);
	movq	%rdx, (%rcx)	 # __source, MEM[(struct ._anon_78 * &)__dest_5(D)]
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/std_function.h:285:       }
	ret	
	.p2align 4,,10
	.p2align 3
.L4:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/std_function.h:274: 	    __dest._M_access<const type_info*>() = &typeid(_Functor);
	leaq	_ZTIZNK5ArrayIdEmlERKS0_EUlddE_(%rip), %rax	 #, tmp95
	movq	%rax, (%rcx)	 # tmp95, MEM[(const struct type_info * &)__dest_5(D)]
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/std_function.h:285:       }
	xorl	%eax, %eax	 #
	ret	
	.seh_endproc
	.text
	.align 2
	.p2align 4
	.def	_ZNSt24uniform_int_distributionIlEclISt26linear_congruential_engineIjLj16807ELj0ELj2147483647EEEElRT_RKNS0_10param_typeE.isra.0;	.scl	3;	.type	32;	.endef
	.seh_proc	_ZNSt24uniform_int_distributionIlEclISt26linear_congruential_engineIjLj16807ELj0ELj2147483647EEEElRT_RKNS0_10param_typeE.isra.0
_ZNSt24uniform_int_distributionIlEclISt26linear_congruential_engineIjLj16807ELj0ELj2147483647EEEElRT_RKNS0_10param_typeE.isra.0:
.LFB6723:
	pushq	%rbp	 #
	.seh_pushreg	%rbp
	pushq	%rdi	 #
	.seh_pushreg	%rdi
	pushq	%rsi	 #
	.seh_pushreg	%rsi
	pushq	%rbx	 #
	.seh_pushreg	%rbx
	subq	$40, %rsp	 #,
	.seh_stackalloc	40
	.seh_endprologue
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:290: 	const __uctype __urange
	subl	%edx, %r8d	 # ISRA.270, tmp211
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:276:       uniform_int_distribution<_IntType>::
	movq	%rcx, %rsi	 # tmp209, __urng
	movl	%edx, %ebx	 # tmp210, ISRA.270
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:290: 	const __uctype __urange
	movl	%r8d, %ebp	 # tmp211, __urange
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:294: 	if (__urngrange > __urange)
	cmpl	$2147483644, %r8d	 #, __urange
	ja	.L8	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:298: 	    const __uctype __uerange = __urange + 1; // __urange can be zero
	leal	1(%r8), %r8d	 #, __uerange
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:323: 		const __uctype __scaling = __urngrange / __uerange;
	movl	$2147483645, %eax	 #, tmp131
	xorl	%edx, %edx	 # tmp130
	movl	(%rcx), %ecx	 # __urng_9(D)->_M_x, __urng___M_x_lsm.273
	divl	%r8d	 # __uerange
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:324: 		const __uctype __past = __uerange * __scaling;
	imull	%eax, %r8d	 # tmp129, __past
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:323: 		const __uctype __scaling = __urngrange / __uerange;
	movl	%eax, %r10d	 # tmp131, tmp129
	.p2align 4,,10
	.p2align 3
.L12:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:61: 	    _Tp __t1 = __a * (__x % __q);
	movl	%ecx, %r9d	 # __urng___M_x_lsm.273, __urng___M_x_lsm.273
	movl	%ecx, %edx	 # __urng___M_x_lsm.273, tmp139
	imulq	$110892733, %r9, %r9	 #, __urng___M_x_lsm.273, tmp137
	shrq	$32, %r9	 #, tmp138
	subl	%r9d, %edx	 # tmp138, tmp139
	shrl	%edx	 # tmp140
	addl	%r9d, %edx	 # tmp138, tmp141
	shrl	$16, %edx	 #, tmp135
	imull	$127773, %edx, %eax	 #, tmp135, tmp142
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:62: 	    _Tp __t2 = __r * (__x / __q);
	imull	$2836, %edx, %edx	 #, tmp135, __t2
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:61: 	    _Tp __t1 = __a * (__x % __q);
	subl	%eax, %ecx	 # tmp142, tmp143
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:61: 	    _Tp __t1 = __a * (__x % __q);
	imull	$16807, %ecx, %ecx	 #, tmp143, __t1
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:63: 	    if (__t1 >= __t2)
	cmpl	%edx, %ecx	 # __t2, __t1
	jnb	.L24	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:66: 	      __x = __m - __t2 + __t1;
	addl	$2147483647, %ecx	 #, tmp151
.L24:
	subl	%edx, %ecx	 # __t2, __urng___M_x_lsm.273
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:326: 		  __ret = __uctype(__urng()) - __urngmin;
	leal	-1(%rcx), %eax	 #, __ret
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:327: 		while (__ret >= __past);
	cmpl	%r8d, %eax	 # __past, __ret
	jnb	.L12	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:328: 		__ret /= __scaling;
	xorl	%edx, %edx	 # tmp153
	movl	%ecx, (%rsi)	 # __urng___M_x_lsm.273, __urng_9(D)->_M_x
	divl	%r10d	 # tmp129
.L13:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:361: 	return __ret + __param.a();
	addl	%ebx, %eax	 # ISRA.270, tmp206
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:362:       }
	addq	$40, %rsp	 #,
	popq	%rbx	 #
	popq	%rsi	 #
	popq	%rdi	 #
	popq	%rbp	 #
	ret	
	.p2align 4,,10
	.p2align 3
.L8:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:331: 	else if (__urngrange < __urange)
	cmpl	$2147483645, %r8d	 #, __urange
	je	.L14	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:353: 			 (__urng, param_type(0, __urange / __uerngrange)));
	movl	%r8d, %eax	 # __urange, tmp154
	shrl	%eax	 #
	movq	%rax, %rdx	 # tmp155, tmp157
	salq	$30, %rdx	 #, tmp157
	addq	%rax, %rdx	 # tmp155, tmp158
	leaq	(%rax,%rdx,2), %rdi	 #, tmp160
	shrq	$61, %rdi	 #, _23
.L18:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:353: 			 (__urng, param_type(0, __urange / __uerngrange)));
	movl	%edi, %r8d	 # _23,
	xorl	%edx, %edx	 #
	movq	%rsi, %rcx	 # __urng,
	call	_ZNSt24uniform_int_distributionIlEclISt26linear_congruential_engineIjLj16807ELj0ELj2147483647EEEElRT_RKNS0_10param_typeE.isra.0	 #
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.h:367: 	_M_x = __detail::__mod<_UIntType, __m, __a, __c>(_M_x);
	movl	(%rsi), %r8d	 # __urng_9(D)->_M_x,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:352: 		__tmp = (__uerngrange * operator()
	imull	$2147483646, %eax, %ecx	 #, tmp212, __tmp
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.h:367: 	_M_x = __detail::__mod<_UIntType, __m, __a, __c>(_M_x);
	movq	%r8, %rax	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:61: 	    _Tp __t1 = __a * (__x % __q);
	imulq	$110892733, %r8, %r8	 #, _27, tmp164
	movl	%eax, %edx	 # _27, tmp166
	shrq	$32, %r8	 #, tmp165
	subl	%r8d, %edx	 # tmp165, tmp166
	shrl	%edx	 # tmp167
	addl	%r8d, %edx	 # tmp165, tmp168
	shrl	$16, %edx	 #, tmp162
	imull	$127773, %edx, %r8d	 #, tmp162, tmp169
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:62: 	    _Tp __t2 = __r * (__x / __q);
	imull	$2836, %edx, %edx	 #, tmp162, __t2
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:61: 	    _Tp __t1 = __a * (__x % __q);
	subl	%r8d, %eax	 # tmp169, tmp170
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:61: 	    _Tp __t1 = __a * (__x % __q);
	imull	$16807, %eax, %eax	 #, tmp170, __t1
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:63: 	    if (__t1 >= __t2)
	cmpl	%edx, %eax	 # __t2, __t1
	jb	.L15	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:64: 	      __x = __t1 - __t2;
	subl	%edx, %eax	 # __t2, __x
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.h:367: 	_M_x = __detail::__mod<_UIntType, __m, __a, __c>(_M_x);
	movl	%eax, (%rsi)	 # __x, __urng_9(D)->_M_x
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:354: 		__ret = __tmp + (__uctype(__urng()) - __urngmin);
	leal	-1(%rcx,%rax), %eax	 #, __ret
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:356: 	    while (__ret > __urange || __ret < __tmp);
	cmpl	%ecx, %eax	 # __tmp, __ret
	jb	.L18	 #,
	cmpl	%eax, %ebp	 # __ret, __urange
	jb	.L18	 #,
	jmp	.L13	 #
	.p2align 4,,10
	.p2align 3
.L15:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:66: 	      __x = __m - __t2 + __t1;
	addl	$2147483647, %eax	 #, tmp183
	subl	%edx, %eax	 # __t2, __x
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.h:367: 	_M_x = __detail::__mod<_UIntType, __m, __a, __c>(_M_x);
	movl	%eax, (%rsi)	 # __x, __urng_9(D)->_M_x
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:354: 		__ret = __tmp + (__uctype(__urng()) - __urngmin);
	leal	-1(%rcx,%rax), %eax	 #, __ret
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:356: 	    while (__ret > __urange || __ret < __tmp);
	cmpl	%eax, %ebp	 # __ret, __urange
	jb	.L18	 #,
	cmpl	%ecx, %eax	 # __tmp, __ret
	jb	.L18	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:361: 	return __ret + __param.a();
	addl	%ebx, %eax	 # ISRA.270, tmp206
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:362:       }
	addq	$40, %rsp	 #,
	popq	%rbx	 #
	popq	%rsi	 #
	popq	%rdi	 #
	popq	%rbp	 #
	ret	
	.p2align 4,,10
	.p2align 3
.L14:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.h:367: 	_M_x = __detail::__mod<_UIntType, __m, __a, __c>(_M_x);
	movl	(%rcx), %eax	 # __urng_9(D)->_M_x,
	movq	%rax, %rcx	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:61: 	    _Tp __t1 = __a * (__x % __q);
	imulq	$110892733, %rax, %rax	 #, _41, tmp191
	movl	%ecx, %edx	 # _41, tmp193
	shrq	$32, %rax	 #, tmp192
	subl	%eax, %edx	 # tmp192, tmp193
	shrl	%edx	 # tmp194
	addl	%eax, %edx	 # tmp192, tmp195
	shrl	$16, %edx	 #, tmp189
	imull	$127773, %edx, %eax	 #, tmp189, tmp196
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:62: 	    _Tp __t2 = __r * (__x / __q);
	imull	$2836, %edx, %edx	 #, tmp189, __t2
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:61: 	    _Tp __t1 = __a * (__x % __q);
	subl	%eax, %ecx	 # tmp196, tmp197
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:61: 	    _Tp __t1 = __a * (__x % __q);
	imull	$16807, %ecx, %ecx	 #, tmp197, __t1
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:64: 	      __x = __t1 - __t2;
	movl	%ecx, %r8d	 # __t1, tmp207
	leal	2147483647(%rcx), %eax	 #, tmp205
	subl	%edx, %r8d	 # __t2, tmp207
	subl	%edx, %eax	 # __t2, tmp208
	cmpl	%edx, %ecx	 # __t2, __t1
	cmovnb	%r8d, %eax	 # tmp208,, tmp207, __x
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.h:367: 	_M_x = __detail::__mod<_UIntType, __m, __a, __c>(_M_x);
	movl	%eax, (%rsi)	 # __x, __urng_9(D)->_M_x
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:359: 	  __ret = __uctype(__urng()) - __urngmin;
	subl	$1, %eax	 #, __ret
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:361: 	return __ret + __param.a();
	addl	%ebx, %eax	 # ISRA.270, tmp206
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:362:       }
	addq	$40, %rsp	 #,
	popq	%rbx	 #
	popq	%rsi	 #
	popq	%rdi	 #
	popq	%rbp	 #
	ret	
	.seh_endproc
	.p2align 4
	.def	_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_.isra.0;	.scl	3;	.type	32;	.endef
	.seh_proc	_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_.isra.0
_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_.isra.0:
.LFB6725:
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
	movq	%rcx, %rbx	 # tmp99, __os
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_ios.h:49:       if (!__f)
	testq	%rsi, %rsi	 # _14
	je	.L29	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/locale_facets.h:882: 	if (_M_widen_ok)
	cmpb	$0, 56(%rsi)	 #, MEM[(const struct ctype *)_14]._M_widen_ok
	je	.L27	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/locale_facets.h:883: 	  return _M_widen[static_cast<unsigned char>(__c)];
	movsbl	67(%rsi), %edx	 # MEM[(const struct ctype *)_14]._M_widen[10],
.L28:
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:736:     { return flush(__os.put(__os.widen('\n'))); }
	movq	%rbx, %rcx	 # __os,
	call	_ZNSo3putEc	 #
	movq	%rax, %rcx	 # tmp101, _8
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:736:     { return flush(__os.put(__os.widen('\n'))); }
	addq	$40, %rsp	 #,
	popq	%rbx	 #
	popq	%rsi	 #
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:758:     { return __os.flush(); }
	jmp	_ZNSo5flushEv	 #
.L27:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/locale_facets.h:884: 	this->_M_widen_init();
	movq	%rsi, %rcx	 # _14,
	call	_ZNKSt5ctypeIcE13_M_widen_initEv	 #
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/locale_facets.h:885: 	return this->do_widen(__c);
	movq	(%rsi), %rax	 # MEM[(const struct ctype *)_14].D.44691._vptr.facet, MEM[(const struct ctype *)_14].D.44691._vptr.facet
	movl	$10, %edx	 #,
	movq	%rsi, %rcx	 # _14,
	call	*48(%rax)	 # MEM[(int (*) () *)_24 + 48B]
	movsbl	%al, %edx	 # tmp100,
	jmp	.L28	 #
.L29:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_ios.h:50: 	__throw_bad_cast();
	call	_ZSt16__throw_bad_castv	 #
	nop	
	.seh_endproc
	.section .rdata,"dr"
.LC0:
	.ascii "data.hpp\0"
	.align 8
.LC1:
	.ascii "(void(\"This Data pointer has already released its data!\"), mControl != nullptr)\0"
	.align 8
.LC2:
	.ascii "(void(\"Data has already been fully released!\"), mAccessCount != 0)\0"
	.section	.text$_ZN4DataIdE7releaseEv,"x"
	.linkonce discard
	.align 2
	.p2align 4
	.globl	_ZN4DataIdE7releaseEv
	.def	_ZN4DataIdE7releaseEv;	.scl	2;	.type	32;	.endef
	.seh_proc	_ZN4DataIdE7releaseEv
_ZN4DataIdE7releaseEv:
.LFB6289:
	pushq	%rsi	 #
	.seh_pushreg	%rsi
	pushq	%rbx	 #
	.seh_pushreg	%rbx
	subq	$40, %rsp	 #,
	.seh_stackalloc	40
	.seh_endprologue
 # data.hpp:157:         assertm(mControl != nullptr, "This Data pointer has already released its data!");
	movq	8(%rcx), %rbx	 # this_6(D)->mControl, _1
 # data.hpp:155:     void release()
	movq	%rcx, %rsi	 # tmp96, this
 # data.hpp:157:         assertm(mControl != nullptr, "This Data pointer has already released its data!");
	testq	%rbx, %rbx	 # _1
	je	.L38	 #,
 # data.hpp:26:             assertm(mAccessCount != 0, "Data has already been fully released!");
	movq	8(%rbx), %rax	 # _1->mAccessCount, _13
	testq	%rax, %rax	 # _13
	je	.L39	 #,
 # data.hpp:27:             mAccessCount--;
	subq	$1, %rax	 #, _14
	movq	%rax, 8(%rbx)	 # _14, _1->mAccessCount
 # data.hpp:29:             if (mAccessCount == 0)
	jne	.L33	 #,
 # data.hpp:31:                 delete[] mRaw;
	movq	(%rbx), %rcx	 # _1->mRaw, _15
	movq	%rbx, %rax	 # _1, pretmp_22
 # data.hpp:31:                 delete[] mRaw;
	testq	%rcx, %rcx	 # _15
	je	.L34	 #,
 # data.hpp:31:                 delete[] mRaw;
	call	_ZdaPv	 #
 # data.hpp:159:         if (mControl->mRealeased)
	movq	8(%rsi), %rax	 # this_6(D)->mControl, pretmp_22
.L34:
 # data.hpp:32:                 mRealeased = true;
	movb	$1, 24(%rbx)	 #, _1->mRealeased
	movq	%rax, %rbx	 # pretmp_22, _1
.L33:
 # data.hpp:159:         if (mControl->mRealeased)
	cmpb	$0, 24(%rbx)	 #, prephitmp_24->mRealeased
	jne	.L40	 #,
 # data.hpp:162:         mRaw = nullptr;
	pxor	%xmm0, %xmm0	 # tmp95
	movups	%xmm0, (%rsi)	 # tmp95, MEM <vector(2) long long unsigned int> [(void *)this_6(D)]
 # data.hpp:163:     }
	addq	$40, %rsp	 #,
	popq	%rbx	 #
	popq	%rsi	 #
	ret	
	.p2align 4,,10
	.p2align 3
.L40:
 # data.hpp:160:             delete mControl;
	movl	$32, %edx	 #,
	movq	%rbx, %rcx	 # _1,
	call	_ZdlPvy	 #
 # data.hpp:162:         mRaw = nullptr;
	pxor	%xmm0, %xmm0	 # tmp95
	movups	%xmm0, (%rsi)	 # tmp95, MEM <vector(2) long long unsigned int> [(void *)this_6(D)]
 # data.hpp:163:     }
	addq	$40, %rsp	 #,
	popq	%rbx	 #
	popq	%rsi	 #
	ret	
.L39:
 # data.hpp:26:             assertm(mAccessCount != 0, "Data has already been fully released!");
	movl	$26, %r8d	 #,
	leaq	.LC0(%rip), %rdx	 #, tmp92
	leaq	.LC2(%rip), %rcx	 #, tmp93
	call	*__imp__assert(%rip)	 #
.L38:
 # data.hpp:157:         assertm(mControl != nullptr, "This Data pointer has already released its data!");
	movl	$157, %r8d	 #,
	leaq	.LC0(%rip), %rdx	 #, tmp89
	leaq	.LC1(%rip), %rcx	 #, tmp90
	call	*__imp__assert(%rip)	 #
	nop	
	.seh_endproc
	.section .rdata,"dr"
	.align 8
.LC3:
	.ascii "Size cannot be bigger than buffer size.\0"
.LC4:
	.ascii "The index is out of range.\0"
	.align 8
.LC5:
	.ascii "(void(\"Index out of bounds!\"), i >= 0 && i < size())\0"
	.section	.text$_ZNK5ArrayIdEplERKS0_,"x"
	.linkonce discard
	.align 2
	.p2align 4
	.globl	_ZNK5ArrayIdEplERKS0_
	.def	_ZNK5ArrayIdEplERKS0_;	.scl	2;	.type	32;	.endef
	.seh_proc	_ZNK5ArrayIdEplERKS0_
_ZNK5ArrayIdEplERKS0_:
.LFB6086:
	pushq	%r15	 #
	.seh_pushreg	%r15
	pushq	%r14	 #
	.seh_pushreg	%r14
	pushq	%r13	 #
	.seh_pushreg	%r13
	pushq	%r12	 #
	.seh_pushreg	%r12
	pushq	%rbp	 #
	.seh_pushreg	%rbp
	pushq	%rdi	 #
	.seh_pushreg	%rdi
	pushq	%rsi	 #
	.seh_pushreg	%rsi
	pushq	%rbx	 #
	.seh_pushreg	%rbx
	subq	$56, %rsp	 #,
	.seh_stackalloc	56
	.seh_endprologue
 # array.hpp:475:         Array<T> result(Data<T>(mFlatLength), mShape);
	movslq	16(%rdx), %rsi	 # this_4(D)->mFlatLength,
 # data.hpp:70:         mRaw = new T[size];
	movq	%rsi, %rax	 # _3, tmp234
 # array.hpp:475:         Array<T> result(Data<T>(mFlatLength), mShape);
	movq	%rsi, %r13	 #,
 # data.hpp:70:         mRaw = new T[size];
	shrq	$60, %rax	 #, tmp234
 # array.hpp:471:     Array<T> operator+(const Array<T> &other) const
	movq	%rcx, %rdi	 # tmp220, <retval>
	movq	%rdx, %r12	 # tmp221, this
	movq	%r8, %r14	 # tmp222, other
 # data.hpp:70:         mRaw = new T[size];
	jne	.L42	 #,
 # data.hpp:70:         mRaw = new T[size];
	leaq	0(,%rsi,8), %rcx	 #, tmp160
.LEHB0:
 # data.hpp:70:         mRaw = new T[size];
	call	_Znay	 #
 # data.hpp:71:         mControl = new Control(mRaw, size);
	movl	$32, %ecx	 #,
 # data.hpp:70:         mRaw = new T[size];
	movq	%rax, %rbx	 # tmp223, _39
 # data.hpp:70:         mRaw = new T[size];
	movq	%rax, 32(%rsp)	 # _39, D.118635.mRaw
 # data.hpp:71:         mControl = new Control(mRaw, size);
	call	_Znwy	 #
.LEHE0:
 # stack_buffer.hpp:74:         mSize = other.mSize;
	movq	56(%r12), %r15	 # MEM[(const struct StackBuffer &)this_4(D) + 24].mSize, _29
 # data.hpp:99:         mRaw = other.mRaw;
	movq	%rbx, (%rdi)	 # _39, MEM[(struct Data *)result_7(D)].mRaw
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/stl_algobase.h:437: 	    __builtin_memmove(__result, __first, sizeof(_Tp) * _Num);
	leaq	24(%r12), %rdx	 #, tmp165
 # data.hpp:39:             mRaw = raw;
	movq	%rbx, (%rax)	 # _39, MEM[(struct Control *)_40].mRaw
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/stl_algobase.h:437: 	    __builtin_memmove(__result, __first, sizeof(_Tp) * _Num);
	leaq	24(%rdi), %rcx	 #, tmp166
	movl	$32, %r8d	 #,
 # data.hpp:71:         mControl = new Control(mRaw, size);
	movq	%rax, %rbp	 # tmp224, _40
 # data.hpp:40:             mSize = size;
	movq	%rsi, 16(%rax)	 # _3, MEM[(struct Control *)_40].mSize
 # data.hpp:42:             mRealeased = false;
	movb	$0, 24(%rax)	 #, MEM[(struct Control *)_40].mRealeased
 # data.hpp:100:         mControl = other.mControl;
	movq	%rax, 8(%rdi)	 # _40, MEM[(struct Data *)result_7(D)].mControl
 # data.hpp:101:         mControl->mAccessCount++;
	movq	$2, 8(%rax)	 #, MEM[(struct Control *)_40].mAccessCount
 # array.hpp:83:     Array<T>(const Data<T> &data, const Coordinates &shape) : mData(data), mFlatLength(mData.size()), mShape(shape), mDim(shape.size()), mStrides(shape.size())
	movl	%esi, 16(%rdi)	 # _2, *result_7(D).mFlatLength
 # stack_buffer.hpp:74:         mSize = other.mSize;
	movq	%r15, 56(%rdi)	 # _29, MEM[(struct StackBuffer *)result_7(D) + 24B].mSize
 # data.hpp:71:         mControl = new Control(mRaw, size);
	movq	%rax, 40(%rsp)	 # _40, D.118635.mControl
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/stl_algobase.h:437: 	    __builtin_memmove(__result, __first, sizeof(_Tp) * _Num);
	call	memmove	 #
 # stack_buffer.hpp:19:     size_t size() const { return mSize; }
	movq	56(%r12), %rax	 # MEM[(const struct StackBuffer *)this_4(D) + 24B].mSize, _33
 # stack_buffer.hpp:28:         if (size > BUFFER_SIZE)
	cmpq	$8, %rax	 #, _33
	ja	.L104	 #,
 # stack_buffer.hpp:30:         mSize = size;
	movq	%rax, 96(%rdi)	 # _33, MEM[(struct StackBuffer *)result_7(D) + 64B].mSize
 # array.hpp:83:     Array<T>(const Data<T> &data, const Coordinates &shape) : mData(data), mFlatLength(mData.size()), mShape(shape), mDim(shape.size()), mStrides(shape.size())
	movb	$0, 104(%rdi)	 #, *result_7(D).transposed
 # array.hpp:83:     Array<T>(const Data<T> &data, const Coordinates &shape) : mData(data), mFlatLength(mData.size()), mShape(shape), mDim(shape.size()), mStrides(shape.size())
	movl	%eax, 108(%rdi)	 # _33, *result_7(D).mDim
 # array.hpp:179:         for (size_t i = 0; i < mDim; i++)
	testq	%rax, %rax	 # _33
	je	.L46	 #,
	movq	%rax, %rcx	 # _33, tmp203
	addq	$-1, %rcx	 #, tmp203
	jc	.L105	 #,
.L62:
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	movl	$16, %ecx	 #,
	call	__cxa_allocate_exception	 #
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	leaq	.LC4(%rip), %rdx	 #, tmp175
	movq	%rax, %rcx	 # _63,
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	movq	%rax, %rsi	 # tmp227, _63
.LEHB1:
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	call	_ZNSt12out_of_rangeC1EPKc	 #
.LEHE1:
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	leaq	_ZNSt12out_of_rangeD1Ev(%rip), %r8	 #,
	leaq	_ZTISt12out_of_range(%rip), %rdx	 #, tmp176
	movq	%rsi, %rcx	 # _63,
.LEHB2:
	call	__cxa_throw	 #
.LEHE2:
.L105:
	leaq	(%rdi,%rax,4), %rdx	 #, _106
 # array.hpp:181:             mStrides[mDim - i - 1] = multiplier;
	movl	$1, 60(%rdx)	 #, MEM[(long int &)_106 + 60]
 # stack_buffer.hpp:125:         if (i >= mSize)
	cmpq	%r15, %rcx	 # _29, tmp203
	jnb	.L63	 #,
 # array.hpp:182:             multiplier *= mShape[mDim - i - 1];
	movslq	20(%rdx), %r8	 # MEM[(long int &)_106 + 20],
 # array.hpp:179:         for (size_t i = 0; i < mDim; i++)
	cmpq	$1, %rax	 #, _33
	je	.L46	 #,
	movq	%rax, %r9	 # _33, tmp182
	addq	$-2, %r9	 #, tmp182
	jnc	.L62	 #,
 # array.hpp:181:             mStrides[mDim - i - 1] = multiplier;
	movl	%r8d, 56(%rdx)	 # _84, MEM[(long int &)_106 + 56]
 # stack_buffer.hpp:125:         if (i >= mSize)
	cmpq	%r15, %r9	 # _29, tmp182
	jnb	.L63	 #,
 # array.hpp:182:             multiplier *= mShape[mDim - i - 1];
	movslq	16(%rdx), %rcx	 # MEM[(long int &)_106 + 16], MEM[(long int &)_106 + 16]
	imulq	%r8, %rcx	 # _48, multiplier
 # array.hpp:179:         for (size_t i = 0; i < mDim; i++)
	cmpq	$2, %rax	 #, _33
	je	.L46	 #,
	movq	%rax, %r8	 # _33, tmp185
	addq	$-3, %r8	 #, tmp185
	jnc	.L62	 #,
 # array.hpp:181:             mStrides[mDim - i - 1] = multiplier;
	movl	%ecx, 52(%rdx)	 # multiplier, MEM[(long int &)_106 + 52]
 # stack_buffer.hpp:125:         if (i >= mSize)
	cmpq	%r15, %r8	 # _29, tmp185
	jnb	.L63	 #,
 # array.hpp:182:             multiplier *= mShape[mDim - i - 1];
	movslq	12(%rdx), %r8	 # MEM[(long int &)_106 + 12], MEM[(long int &)_106 + 12]
	imulq	%rcx, %r8	 # multiplier, multiplier
 # array.hpp:179:         for (size_t i = 0; i < mDim; i++)
	cmpq	$3, %rax	 #, _33
	je	.L46	 #,
	movq	%rax, %rcx	 # _33, tmp188
	addq	$-4, %rcx	 #, tmp188
	jnc	.L62	 #,
 # array.hpp:181:             mStrides[mDim - i - 1] = multiplier;
	movl	%r8d, 48(%rdx)	 # multiplier, MEM[(long int &)_106 + 48]
 # stack_buffer.hpp:125:         if (i >= mSize)
	cmpq	%r15, %rcx	 # _29, tmp188
	jnb	.L63	 #,
 # array.hpp:182:             multiplier *= mShape[mDim - i - 1];
	movslq	8(%rdx), %rcx	 # MEM[(long int &)_106 + 8], MEM[(long int &)_106 + 8]
	imulq	%r8, %rcx	 # multiplier, multiplier
 # array.hpp:179:         for (size_t i = 0; i < mDim; i++)
	cmpq	$4, %rax	 #, _33
	je	.L46	 #,
 # array.hpp:181:             mStrides[mDim - i - 1] = multiplier;
	leaq	-5(%rax), %r8	 #, tmp191
	movl	%ecx, 44(%rdx)	 # multiplier, MEM[(long int &)_106 + 44]
 # stack_buffer.hpp:125:         if (i >= mSize)
	cmpq	%r15, %r8	 # _29, tmp191
	jnb	.L63	 #,
 # array.hpp:182:             multiplier *= mShape[mDim - i - 1];
	movslq	4(%rdx), %r8	 # MEM[(long int &)_106 + 4], MEM[(long int &)_106 + 4]
	imulq	%r8, %rcx	 # MEM[(long int &)_106 + 4], multiplier
 # array.hpp:179:         for (size_t i = 0; i < mDim; i++)
	cmpq	$5, %rax	 #, _33
	je	.L46	 #,
 # array.hpp:181:             mStrides[mDim - i - 1] = multiplier;
	leaq	-6(%rax), %r8	 #, tmp194
	movl	%ecx, 40(%rdx)	 # multiplier, MEM[(long int &)_106 + 40]
 # stack_buffer.hpp:125:         if (i >= mSize)
	cmpq	%r15, %r8	 # _29, tmp194
	jnb	.L63	 #,
 # array.hpp:182:             multiplier *= mShape[mDim - i - 1];
	movslq	(%rdx), %r8	 # *_106, *_106
	imulq	%r8, %rcx	 # *_106, multiplier
 # array.hpp:179:         for (size_t i = 0; i < mDim; i++)
	cmpq	$6, %rax	 #, _33
	je	.L46	 #,
 # array.hpp:181:             mStrides[mDim - i - 1] = multiplier;
	xorl	%r8d, %r8d	 # tmp197
	cmpq	$7, %rax	 #, _33
	movl	%ecx, 36(%rdx)	 # multiplier, MEM[(long int &)_106 + 36]
	setne	%r8b	 #, tmp197
 # stack_buffer.hpp:125:         if (i >= mSize)
	cmpq	%r15, %r8	 # _29, tmp197
	jnb	.L63	 #,
 # array.hpp:182:             multiplier *= mShape[mDim - i - 1];
	movslq	-4(%rdx), %r8	 # MEM[(long int &)_106 + 18446744073709551612], MEM[(long int &)_106 + 18446744073709551612]
	imulq	%rcx, %r8	 # multiplier, multiplier
 # array.hpp:179:         for (size_t i = 0; i < mDim; i++)
	cmpq	$8, %rax	 #, _33
	jne	.L46	 #,
 # array.hpp:181:             mStrides[mDim - i - 1] = multiplier;
	movl	%r8d, 32(%rdx)	 # multiplier, MEM[(long int &)_106 + 32]
.L46:
 # data.hpp:27:             mAccessCount--;
	movq	$1, 8(%rbp)	 #, MEM[(struct Control *)_40].mAccessCount
 # array.hpp:463:         for (size_t k = 0; k < mFlatLength; k++)
	testl	%r13d, %r13d	 # _2
	je	.L41	 #,
 # data.hpp:65:     size_t size() const { return mControl->mSize; }
	movq	8(%r14), %rax	 # MEM[(struct Control * *)other_10(D) + 8B], MEM[(struct Control * *)other_10(D) + 8B]
	movq	(%r14), %rcx	 # MEM[(double * *)other_10(D)], _197
	movq	16(%rax), %rdx	 # _189->mSize, _122
 # array.hpp:463:         for (size_t k = 0; k < mFlatLength; k++)
	xorl	%eax, %eax	 # k
	.p2align 4,,10
	.p2align 3
.L69:
 # data.hpp:152:         assertm(i >= 0 && i < size(), "Index out of bounds!");
	cmpq	%rax, %rdx	 # k, _122
	je	.L106	 #,
 # array.hpp:464:             mData[k] = swap ? f(other.mData[k], mData[k]) : f(mData[k], other.mData[k]);
	movsd	(%rcx,%rax,8), %xmm0	 # MEM[(double &)_197 + k_184 * 8], _86
 # data.hpp:152:         assertm(i >= 0 && i < size(), "Index out of bounds!");
	cmpq	%rax, %rsi	 # k, _3
	je	.L107	 #,
 # array.hpp:469:     static inline T add(T a, T b) { return a + b; }
	addsd	(%rbx,%rax,8), %xmm0	 # MEM[(double &)_39 + k_184 * 8], tmp216
 # array.hpp:464:             mData[k] = swap ? f(other.mData[k], mData[k]) : f(mData[k], other.mData[k]);
	movsd	%xmm0, (%rbx,%rax,8)	 # tmp216, MEM[(double &)_39 + k_184 * 8]
 # array.hpp:463:         for (size_t k = 0; k < mFlatLength; k++)
	addq	$1, %rax	 #, k
 # array.hpp:463:         for (size_t k = 0; k < mFlatLength; k++)
	cmpq	%rax, %rsi	 # k, _3
	jne	.L69	 #,
.L41:
 # array.hpp:479:     }
	movq	%rdi, %rax	 # <retval>,
	addq	$56, %rsp	 #,
	popq	%rbx	 #
	popq	%rsi	 #
	popq	%rdi	 #
	popq	%rbp	 #
	popq	%r12	 #
	popq	%r13	 #
	popq	%r14	 #
	popq	%r15	 #
	ret	
.L106:
 # data.hpp:152:         assertm(i >= 0 && i < size(), "Index out of bounds!");
	movl	$152, %r8d	 #,
	leaq	.LC0(%rip), %rdx	 #, tmp210
	leaq	.LC5(%rip), %rcx	 #, tmp211
.LEHB3:
	call	*__imp__assert(%rip)	 #
.L107:
	movl	$152, %r8d	 #,
	leaq	.LC0(%rip), %rdx	 #, tmp213
	leaq	.LC5(%rip), %rcx	 #, tmp214
	call	*__imp__assert(%rip)	 #
.LEHE3:
.L73:
 # array.hpp:66:     ~Array() = default;
	movq	%rax, %rbx	 # tmp232, tmp218
	jmp	.L71	 #
.L63:
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	movl	$16, %ecx	 #,
	call	__cxa_allocate_exception	 #
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	leaq	.LC4(%rip), %rdx	 #, tmp180
	movq	%rax, %rcx	 # _66,
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	movq	%rax, %rsi	 # tmp229, _66
.LEHB4:
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	call	_ZNSt12out_of_rangeC1EPKc	 #
.LEHE4:
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	leaq	_ZNSt12out_of_rangeD1Ev(%rip), %r8	 #,
	leaq	_ZTISt12out_of_range(%rip), %rdx	 #, tmp181
	movq	%rsi, %rcx	 # _66,
.LEHB5:
	call	__cxa_throw	 #
.LEHE5:
.L71:
 # data.hpp:167:         if (mControl != nullptr)
	cmpq	$0, 8(%rdi)	 #, MEM[(struct Data *)result_7(D)].mControl
	je	.L72	 #,
 # data.hpp:168:             release();
	movq	%rdi, %rcx	 # <retval>,
	call	_ZN4DataIdE7releaseEv	 #
.L72:
	movq	%rbx, %rcx	 # tmp218,
.LEHB6:
	call	_Unwind_Resume	 #
.L77:
.L103:
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	movq	%rax, %rbx	 # tmp230, tmp201
	jmp	.L59	 #
.L42:
 # data.hpp:70:         mRaw = new T[size];
	call	__cxa_throw_bad_array_new_length	 #
.LEHE6:
.L74:
 # array.hpp:86:     }
	movq	%rax, %rbx	 # tmp231, tmp170
	jmp	.L48	 #
.L76:
	jmp	.L103	 #
.L59:
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	movq	%rsi, %rcx	 # _66,
	call	__cxa_free_exception	 #
.L48:
 # data.hpp:167:         if (mControl != nullptr)
	cmpq	$0, 8(%rdi)	 #, MEM[(struct Data *)result_7(D)].mControl
	je	.L65	 #,
 # data.hpp:168:             release();
	movq	%rdi, %rcx	 # <retval>,
	call	_ZN4DataIdE7releaseEv	 #
.L65:
	leaq	32(%rsp), %rcx	 #, tmp207
	call	_ZN4DataIdE7releaseEv	 #
	movq	%rbx, %rcx	 # tmp170,
.LEHB7:
	call	_Unwind_Resume	 #
.LEHE7:
.L104:
 # stack_buffer.hpp:29:             throw std::invalid_argument("Size cannot be bigger than buffer size.");
	movl	$16, %ecx	 #,
	call	__cxa_allocate_exception	 #
 # stack_buffer.hpp:29:             throw std::invalid_argument("Size cannot be bigger than buffer size.");
	leaq	.LC3(%rip), %rdx	 #, tmp168
	movq	%rax, %rcx	 # _42,
 # stack_buffer.hpp:29:             throw std::invalid_argument("Size cannot be bigger than buffer size.");
	movq	%rax, %rsi	 # tmp225, _42
.LEHB8:
 # stack_buffer.hpp:29:             throw std::invalid_argument("Size cannot be bigger than buffer size.");
	call	_ZNSt16invalid_argumentC1EPKc	 #
.LEHE8:
 # stack_buffer.hpp:29:             throw std::invalid_argument("Size cannot be bigger than buffer size.");
	leaq	_ZNSt16invalid_argumentD1Ev(%rip), %r8	 #,
	leaq	_ZTISt16invalid_argument(%rip), %rdx	 #, tmp169
	movq	%rsi, %rcx	 # _42,
.LEHB9:
	call	__cxa_throw	 #
.LEHE9:
.L75:
	jmp	.L103	 #
	.def	__gxx_personality_seh0;	.scl	2;	.type	32;	.endef
	.seh_handler	__gxx_personality_seh0, @unwind, @except
	.seh_handlerdata
.LLSDA6086:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE6086-.LLSDACSB6086
.LLSDACSB6086:
	.uleb128 .LEHB0-.LFB6086
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB6086
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L76-.LFB6086
	.uleb128 0
	.uleb128 .LEHB2-.LFB6086
	.uleb128 .LEHE2-.LEHB2
	.uleb128 .L74-.LFB6086
	.uleb128 0
	.uleb128 .LEHB3-.LFB6086
	.uleb128 .LEHE3-.LEHB3
	.uleb128 .L73-.LFB6086
	.uleb128 0
	.uleb128 .LEHB4-.LFB6086
	.uleb128 .LEHE4-.LEHB4
	.uleb128 .L77-.LFB6086
	.uleb128 0
	.uleb128 .LEHB5-.LFB6086
	.uleb128 .LEHE5-.LEHB5
	.uleb128 .L74-.LFB6086
	.uleb128 0
	.uleb128 .LEHB6-.LFB6086
	.uleb128 .LEHE6-.LEHB6
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB7-.LFB6086
	.uleb128 .LEHE7-.LEHB7
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB8-.LFB6086
	.uleb128 .LEHE8-.LEHB8
	.uleb128 .L75-.LFB6086
	.uleb128 0
	.uleb128 .LEHB9-.LFB6086
	.uleb128 .LEHE9-.LEHB9
	.uleb128 .L74-.LFB6086
	.uleb128 0
.LLSDACSE6086:
	.section	.text$_ZNK5ArrayIdEplERKS0_,"x"
	.linkonce discard
	.seh_endproc
	.section .rdata,"dr"
.LC6:
	.ascii "array.hpp\0"
	.align 8
.LC7:
	.ascii "(void(\"The shape of the other array is not the same as the shape of this array.\"), isShapeMatch(mShape, other.mShape))\0"
	.section	.text$_ZNK5ArrayIdEmlERKS0_,"x"
	.linkonce discard
	.align 2
	.p2align 4
	.globl	_ZNK5ArrayIdEmlERKS0_
	.def	_ZNK5ArrayIdEmlERKS0_;	.scl	2;	.type	32;	.endef
	.seh_proc	_ZNK5ArrayIdEmlERKS0_
_ZNK5ArrayIdEmlERKS0_:
.LFB6090:
	pushq	%r15	 #
	.seh_pushreg	%r15
	pushq	%r14	 #
	.seh_pushreg	%r14
	pushq	%r13	 #
	.seh_pushreg	%r13
	pushq	%r12	 #
	.seh_pushreg	%r12
	pushq	%rbp	 #
	.seh_pushreg	%rbp
	pushq	%rdi	 #
	.seh_pushreg	%rdi
	pushq	%rsi	 #
	.seh_pushreg	%rsi
	pushq	%rbx	 #
	.seh_pushreg	%rbx
	subq	$120, %rsp	 #,
	.seh_stackalloc	120
	movaps	%xmm6, 96(%rsp)	 #,
	.seh_savexmm	%xmm6, 96
	.seh_endprologue
	leaq	_ZNSt17_Function_handlerIFdddEZNK5ArrayIdEmlERKS2_EUlddE_E9_M_invokeERKSt9_Any_dataOdSA_(%rip), %rax	 #, tmp155
	leaq	_ZNSt17_Function_handlerIFdddEZNK5ArrayIdEmlERKS2_EUlddE_E10_M_managerERSt9_Any_dataRKS7_St18_Manager_operation(%rip), %r13	 #, _9
	movq	%rax, %xmm2	 # tmp155, tmp155
	movq	%r13, %xmm6	 # _9, _168
 # data.hpp:65:     size_t size() const { return mControl->mSize; }
	movq	8(%rdx), %rax	 # MEM[(const struct Data *)this_3(D)].mControl, MEM[(const struct Data *)this_3(D)].mControl
	punpcklqdq	%xmm2, %xmm6	 # tmp155, _168
	movq	16(%rax), %r9	 # _88->mSize, _174
 # data.hpp:146:         T *newRaw = new T[s];
	movq	%r9, %rax	 # _174, tmp231
 # array.hpp:503:     Array<T> operator*(const Array<T> &other) const
	movq	%rcx, %rsi	 # tmp219, <retval>
	movq	%rdx, %rbx	 # tmp220, this
	movq	%r8, %rdi	 # tmp221, other
 # data.hpp:146:         T *newRaw = new T[s];
	shrq	$60, %rax	 #, tmp231
	jne	.L109	 #,
 # data.hpp:146:         T *newRaw = new T[s];
	leaq	0(,%r9,8), %rcx	 #, tmp160
	movq	%r9, 32(%rsp)	 # _174, %sfp
.LEHB10:
 # data.hpp:146:         T *newRaw = new T[s];
	call	_Znay	 #
 # data.hpp:147:         std::copy(mRaw, mRaw + size(), newRaw);
	movq	(%rbx), %rdx	 # MEM[(const struct Data *)this_3(D)].mRaw, _92
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/stl_algobase.h:436: 	  if (__builtin_expect(_Num > 1, true))
	movq	32(%rsp), %r9	 # %sfp, _174
 # data.hpp:146:         T *newRaw = new T[s];
	movq	%rax, %r10	 # tmp222, prephitmp_29
 # data.hpp:65:     size_t size() const { return mControl->mSize; }
	movq	8(%rbx), %rax	 # MEM[(const struct Data *)this_3(D)].mControl, MEM[(const struct Data *)this_3(D)].mControl
 # data.hpp:147:         std::copy(mRaw, mRaw + size(), newRaw);
	movq	16(%rax), %r8	 # _93->mSize, _93->mSize
	salq	$3, %r8	 #, _95
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/stl_algobase.h:436: 	  if (__builtin_expect(_Num > 1, true))
	cmpq	$8, %r8	 #, _95
	jle	.L110	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/stl_algobase.h:437: 	    __builtin_memmove(__result, __first, sizeof(_Tp) * _Num);
	movq	%r10, %rcx	 # prephitmp_29,
	call	memmove	 #
	movq	32(%rsp), %r9	 # %sfp, _174
	movq	%rax, %r10	 #, prephitmp_29
.L111:
 # data.hpp:77:         mControl = new Control(raw, size);
	movl	$32, %ecx	 #,
	movq	%r9, 40(%rsp)	 # _174, %sfp
	movq	%r10, 32(%rsp)	 # prephitmp_29, %sfp
	call	_Znwy	 #
.LEHE10:
 # data.hpp:40:             mSize = size;
	movq	40(%rsp), %r9	 # %sfp, _174
 # data.hpp:39:             mRaw = raw;
	movq	32(%rsp), %r10	 # %sfp, prephitmp_29
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/stl_algobase.h:437: 	    __builtin_memmove(__result, __first, sizeof(_Tp) * _Num);
	leaq	24(%rbx), %rdx	 #, tmp167
 # stack_buffer.hpp:74:         mSize = other.mSize;
	movq	56(%rbx), %rbp	 # MEM[(const struct StackBuffer &)this_3(D) + 24].mSize, _79
 # data.hpp:42:             mRealeased = false;
	movb	$0, 24(%rax)	 #, MEM[(struct Control *)_103].mRealeased
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/stl_algobase.h:437: 	    __builtin_memmove(__result, __first, sizeof(_Tp) * _Num);
	leaq	24(%rsi), %rcx	 #, tmp168
 # data.hpp:77:         mControl = new Control(raw, size);
	movq	%rax, %r12	 # tmp223, tmp164
 # data.hpp:39:             mRaw = raw;
	movq	%r10, (%rax)	 # prephitmp_29, MEM[(struct Control *)_103].mRaw
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/stl_algobase.h:437: 	    __builtin_memmove(__result, __first, sizeof(_Tp) * _Num);
	movl	$32, %r8d	 #,
 # array.hpp:78:     Array<T>(const Data<T> &data, const Coordinates &shape, const Coordinates &strides) : mData(data), mFlatLength(mData.size()), mDim(shape.size()), mShape(shape), mStrides(strides)
	movl	%r9d, %r14d	 # _174, _78
 # data.hpp:40:             mSize = size;
	movq	%r9, 16(%rax)	 # _174, MEM[(struct Control *)_103].mSize
 # data.hpp:99:         mRaw = other.mRaw;
	movq	%r10, (%rsi)	 # prephitmp_29, MEM[(struct Data *)result_2(D)].mRaw
 # array.hpp:78:     Array<T>(const Data<T> &data, const Coordinates &shape, const Coordinates &strides) : mData(data), mFlatLength(mData.size()), mDim(shape.size()), mShape(shape), mStrides(strides)
	movl	%r9d, 16(%rsi)	 # _174, *result_2(D).mFlatLength
 # data.hpp:100:         mControl = other.mControl;
	movq	%rax, 8(%rsi)	 # tmp164, MEM[(struct Data *)result_2(D)].mControl
 # stack_buffer.hpp:74:         mSize = other.mSize;
	movq	%rbp, 56(%rsi)	 # _79, MEM[(struct StackBuffer *)result_2(D) + 24B].mSize
 # data.hpp:99:         mRaw = other.mRaw;
	movq	%r10, 40(%rsp)	 # prephitmp_29, %sfp
 # array.hpp:78:     Array<T>(const Data<T> &data, const Coordinates &shape, const Coordinates &strides) : mData(data), mFlatLength(mData.size()), mDim(shape.size()), mShape(shape), mStrides(strides)
	movq	%r9, 32(%rsp)	 # _174, %sfp
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/stl_algobase.h:437: 	    __builtin_memmove(__result, __first, sizeof(_Tp) * _Num);
	call	memmove	 #
 # stack_buffer.hpp:74:         mSize = other.mSize;
	movq	96(%rbx), %rax	 # MEM[(const struct StackBuffer &)this_3(D) + 64].mSize, MEM[(const struct StackBuffer &)this_3(D) + 64].mSize
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/stl_algobase.h:437: 	    __builtin_memmove(__result, __first, sizeof(_Tp) * _Num);
	leaq	64(%rbx), %rdx	 #, tmp172
	leaq	64(%rsi), %rcx	 #, tmp173
	movl	$32, %r8d	 #,
 # stack_buffer.hpp:74:         mSize = other.mSize;
	movq	%rax, 96(%rsi)	 # MEM[(const struct StackBuffer &)this_3(D) + 64].mSize, MEM[(struct StackBuffer *)result_2(D) + 64B].mSize
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/stl_algobase.h:437: 	    __builtin_memmove(__result, __first, sizeof(_Tp) * _Num);
	call	memmove	 #
 # array.hpp:78:     Array<T>(const Data<T> &data, const Coordinates &shape, const Coordinates &strides) : mData(data), mFlatLength(mData.size()), mDim(shape.size()), mShape(shape), mStrides(strides)
	movq	56(%rbx), %rax	 # MEM[(const struct StackBuffer *)this_3(D) + 24B].mSize, MEM[(const struct StackBuffer *)this_3(D) + 24B].mSize
 # array.hpp:78:     Array<T>(const Data<T> &data, const Coordinates &shape, const Coordinates &strides) : mData(data), mFlatLength(mData.size()), mDim(shape.size()), mShape(shape), mStrides(strides)
	movb	$0, 104(%rsi)	 #, *result_2(D).transposed
 # array.hpp:423:         for (size_t i = 0; i < minDim; i++)
	movq	32(%rsp), %r9	 # %sfp, _174
	movq	40(%rsp), %r10	 # %sfp, prephitmp_29
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/std_function.h:452: 	      _M_manager = &_My_handler::_M_manager;
	movaps	%xmm6, 80(%rsp)	 # _168, MEM <vector(2) long long unsigned int> [(void *)&D.119191 + 16B]
 # array.hpp:78:     Array<T>(const Data<T> &data, const Coordinates &shape, const Coordinates &strides) : mData(data), mFlatLength(mData.size()), mDim(shape.size()), mShape(shape), mStrides(strides)
	movl	%eax, 108(%rsi)	 # MEM[(const struct StackBuffer *)this_3(D) + 24B].mSize, *result_2(D).mDim
 # stack_buffer.hpp:19:     size_t size() const { return mSize; }
	movq	56(%rdi), %rax	 # MEM[(const struct StackBuffer *)other_5(D) + 24B].mSize, _112
 # data.hpp:27:             mAccessCount--;
	movq	$1, 8(%r12)	 #, MEM[(struct Control *)_103].mAccessCount
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/std_function.h:437: 	: _Function_base()
	movq	$0, 64(%rsp)	 #, MEM <char[16]> [(struct _Function_base *)&D.119191]
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/stl_algobase.h:238:       if (__b < __a)
	cmpl	%eax, %ebp	 # tmp230, _79
	movl	%eax, %ecx	 # _112, tmp230
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/std_function.h:437: 	: _Function_base()
	movq	$0, 72(%rsp)	 #, MEM <char[16]> [(struct _Function_base *)&D.119191]
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/stl_algobase.h:238:       if (__b < __a)
	cmovle	%ebp, %ecx	 # _79,, _132
 # array.hpp:423:         for (size_t i = 0; i < minDim; i++)
	testl	%ecx, %ecx	 # _132
 # array.hpp:423:         for (size_t i = 0; i < minDim; i++)
	movslq	%ecx, %rdx	 # _132, _196
 # array.hpp:423:         for (size_t i = 0; i < minDim; i++)
	je	.L113	 #,
 # stack_buffer.hpp:133:         if (i >= mSize)
	testq	%rbp, %rbp	 # _79
	je	.L114	 #,
 # array.hpp:425:             if (shape1[i] != shape2[i])
	movl	24(%rsi), %ecx	 # MEM <const struct StackBuffer> [(const long int &)result_2(D) + 24].mBuffer[0], _114
 # stack_buffer.hpp:133:         if (i >= mSize)
	testq	%rax, %rax	 # _112
	je	.L115	 #,
 # array.hpp:425:             if (shape1[i] != shape2[i])
	movl	24(%rdi), %r8d	 # MEM <const struct StackBuffer> [(const long int &)other_5(D) + 24].mBuffer[0], _221
 # array.hpp:425:             if (shape1[i] != shape2[i])
	cmpl	%r8d, %ecx	 # _221, _114
	jne	.L246	 #,
.L119:
 # array.hpp:423:         for (size_t i = 0; i < minDim; i++)
	cmpq	$1, %rdx	 #, _196
	je	.L113	 #,
 # stack_buffer.hpp:133:         if (i >= mSize)
	cmpq	$1, %rbp	 #, _79
	je	.L114	 #,
 # array.hpp:425:             if (shape1[i] != shape2[i])
	movl	28(%rsi), %ecx	 # MEM <const struct StackBuffer> [(const long int &)result_2(D) + 24].mBuffer[1], _200
 # stack_buffer.hpp:133:         if (i >= mSize)
	cmpq	$1, %rax	 #, _112
	je	.L115	 #,
 # array.hpp:425:             if (shape1[i] != shape2[i])
	movl	28(%rdi), %r8d	 # MEM <const struct StackBuffer> [(const long int &)other_5(D) + 24].mBuffer[1], _184
 # array.hpp:425:             if (shape1[i] != shape2[i])
	cmpl	%ecx, %r8d	 # _200, _184
	jne	.L247	 #,
.L121:
 # array.hpp:423:         for (size_t i = 0; i < minDim; i++)
	cmpq	$2, %rdx	 #, _196
	je	.L113	 #,
 # stack_buffer.hpp:133:         if (i >= mSize)
	cmpq	$2, %rbp	 #, _79
	je	.L114	 #,
 # array.hpp:425:             if (shape1[i] != shape2[i])
	movl	32(%rsi), %ecx	 # MEM <const struct StackBuffer> [(const long int &)result_2(D) + 24].mBuffer[2], _117
 # stack_buffer.hpp:133:         if (i >= mSize)
	cmpq	$2, %rax	 #, _112
	je	.L115	 #,
 # array.hpp:425:             if (shape1[i] != shape2[i])
	movl	32(%rdi), %r8d	 # MEM <const struct StackBuffer> [(const long int &)other_5(D) + 24].mBuffer[2], _183
 # array.hpp:425:             if (shape1[i] != shape2[i])
	cmpl	%r8d, %ecx	 # _183, _117
	jne	.L248	 #,
.L122:
 # array.hpp:423:         for (size_t i = 0; i < minDim; i++)
	cmpq	$3, %rdx	 #, _196
	je	.L113	 #,
 # stack_buffer.hpp:133:         if (i >= mSize)
	cmpq	$3, %rbp	 #, _79
	je	.L114	 #,
 # array.hpp:425:             if (shape1[i] != shape2[i])
	movl	36(%rsi), %ecx	 # MEM <const struct StackBuffer> [(const long int &)result_2(D) + 24].mBuffer[3], _210
 # stack_buffer.hpp:133:         if (i >= mSize)
	cmpq	$3, %rax	 #, _112
	je	.L115	 #,
 # array.hpp:425:             if (shape1[i] != shape2[i])
	movl	36(%rdi), %r8d	 # MEM <const struct StackBuffer> [(const long int &)other_5(D) + 24].mBuffer[3], _76
 # array.hpp:425:             if (shape1[i] != shape2[i])
	cmpl	%ecx, %r8d	 # _210, _76
	jne	.L249	 #,
.L123:
 # array.hpp:423:         for (size_t i = 0; i < minDim; i++)
	cmpq	$4, %rdx	 #, _196
	je	.L113	 #,
 # stack_buffer.hpp:133:         if (i >= mSize)
	cmpq	$4, %rbp	 #, _79
	je	.L114	 #,
 # array.hpp:425:             if (shape1[i] != shape2[i])
	movl	40(%rsi), %ecx	 # MEM <const struct StackBuffer> [(const long int &)result_2(D) + 24].mBuffer[4], _108
 # stack_buffer.hpp:133:         if (i >= mSize)
	cmpq	$4, %rax	 #, _112
	je	.L115	 #,
 # array.hpp:425:             if (shape1[i] != shape2[i])
	movl	40(%rdi), %r8d	 # MEM <const struct StackBuffer> [(const long int &)other_5(D) + 24].mBuffer[4], _87
 # array.hpp:425:             if (shape1[i] != shape2[i])
	cmpl	%ecx, %r8d	 # _108, _87
	jne	.L250	 #,
.L124:
 # array.hpp:423:         for (size_t i = 0; i < minDim; i++)
	cmpq	$5, %rdx	 #, _196
	je	.L113	 #,
 # stack_buffer.hpp:133:         if (i >= mSize)
	cmpq	$5, %rbp	 #, _79
	je	.L114	 #,
 # array.hpp:425:             if (shape1[i] != shape2[i])
	movl	44(%rsi), %ecx	 # MEM <const struct StackBuffer> [(const long int &)result_2(D) + 24].mBuffer[5], _145
 # stack_buffer.hpp:133:         if (i >= mSize)
	cmpq	$5, %rax	 #, _112
	je	.L115	 #,
 # array.hpp:425:             if (shape1[i] != shape2[i])
	movl	44(%rdi), %r8d	 # MEM <const struct StackBuffer> [(const long int &)other_5(D) + 24].mBuffer[5], _52
 # array.hpp:425:             if (shape1[i] != shape2[i])
	cmpl	%ecx, %r8d	 # _145, _52
	jne	.L251	 #,
.L125:
 # array.hpp:423:         for (size_t i = 0; i < minDim; i++)
	cmpq	$6, %rdx	 #, _196
	je	.L113	 #,
 # stack_buffer.hpp:133:         if (i >= mSize)
	cmpq	$6, %rbp	 #, _79
	je	.L114	 #,
 # array.hpp:425:             if (shape1[i] != shape2[i])
	movl	48(%rsi), %ecx	 # MEM <const struct StackBuffer> [(const long int &)result_2(D) + 24].mBuffer[6], _101
 # stack_buffer.hpp:133:         if (i >= mSize)
	cmpq	$6, %rax	 #, _112
	je	.L115	 #,
 # array.hpp:425:             if (shape1[i] != shape2[i])
	movl	48(%rdi), %r8d	 # MEM <const struct StackBuffer> [(const long int &)other_5(D) + 24].mBuffer[6], _246
 # array.hpp:425:             if (shape1[i] != shape2[i])
	cmpl	%r8d, %ecx	 # _246, _101
	jne	.L252	 #,
.L126:
 # array.hpp:423:         for (size_t i = 0; i < minDim; i++)
	cmpq	$7, %rdx	 #, _196
	je	.L113	 #,
 # stack_buffer.hpp:133:         if (i >= mSize)
	cmpq	$7, %rbp	 #, _79
	je	.L114	 #,
 # array.hpp:425:             if (shape1[i] != shape2[i])
	movl	52(%rsi), %ecx	 # MEM <const struct StackBuffer> [(const long int &)result_2(D) + 24].mBuffer[7], _53
 # stack_buffer.hpp:133:         if (i >= mSize)
	cmpq	$7, %rax	 #, _112
	je	.L115	 #,
 # array.hpp:425:             if (shape1[i] != shape2[i])
	movl	52(%rdi), %eax	 # MEM <const struct StackBuffer> [(const long int &)other_5(D) + 24].mBuffer[7], _122
 # array.hpp:425:             if (shape1[i] != shape2[i])
	cmpl	%eax, %ecx	 # _122, _53
	jne	.L253	 #,
.L128:
 # array.hpp:423:         for (size_t i = 0; i < minDim; i++)
	cmpq	$8, %rdx	 #, _196
	jne	.L114	 #,
.L113:
 # array.hpp:333:         for (size_t k = 0; k < mFlatLength; k++)
	testl	%r14d, %r14d	 # _78
	je	.L140	 #,
	movq	(%rdi), %rdx	 # MEM[(double * *)other_5(D)], _60
	movq	8(%rdi), %rax	 # MEM[(struct Control * *)other_5(D) + 8B], _61
 # array.hpp:333:         for (size_t k = 0; k < mFlatLength; k++)
	xorl	%ebx, %ebx	 # k
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/std_function.h:591: 	return _M_invoker(_M_functor, std::forward<_ArgTypes>(__args)...);
	leaq	48(%rsp), %r13	 #, tmp216
	leaq	56(%rsp), %r14	 #, tmp217
 # data.hpp:152:         assertm(i >= 0 && i < size(), "Index out of bounds!");
	movl	$2147483648, %r15d	 #, tmp218
	jmp	.L131	 #
	.p2align 4,,10
	.p2align 3
.L136:
	movq	(%rdi), %rdx	 # MEM[(double * *)other_5(D)], _60
	movq	8(%rdi), %rax	 # MEM[(struct Control * *)other_5(D) + 8B], _61
	cmpq	%r15, %rbx	 # tmp218, k
	je	.L132	 #,
.L131:
 # data.hpp:152:         assertm(i >= 0 && i < size(), "Index out of bounds!");
	cmpq	16(%rax), %rbx	 # _233->mSize, k
	jnb	.L132	 #,
 # array.hpp:334:             mData[k] = swap ? f(other.mData[k], mData[k]) : f(mData[k], other.mData[k]);
	movsd	(%rdx,%rbx,8), %xmm0	 # *_165, _43
	leaq	0(,%rbx,8), %r12	 #, _17
 # data.hpp:152:         assertm(i >= 0 && i < size(), "Index out of bounds!");
	cmpq	%r9, %rbx	 # _174, k
	jnb	.L254	 #,
 # array.hpp:334:             mData[k] = swap ? f(other.mData[k], mData[k]) : f(mData[k], other.mData[k]);
	movsd	(%r10,%rbx,8), %xmm1	 # *_171, _46
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/std_function.h:589: 	if (_M_empty())
	cmpq	$0, 80(%rsp)	 #, MEM[(const struct _Function_base *)&D.119191]._M_manager
	movsd	%xmm0, 48(%rsp)	 # _43, __args#1
	movsd	%xmm1, 56(%rsp)	 # _46, __args#0
	je	.L255	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/std_function.h:591: 	return _M_invoker(_M_functor, std::forward<_ArgTypes>(__args)...);
	leaq	64(%rsp), %rbp	 #, tmp215
	movq	%r13, %r8	 # tmp216,
	movq	%r14, %rdx	 # tmp217,
	movq	%rbp, %rcx	 # tmp215,
.LEHB11:
	call	*88(%rsp)	 # D.119191._M_invoker
.LEHE11:
 # data.hpp:65:     size_t size() const { return mControl->mSize; }
	movq	8(%rsi), %rax	 # MEM[(struct Control * *)result_2(D) + 8B], MEM[(struct Control * *)result_2(D) + 8B]
	movq	(%rsi), %r10	 # MEM[(double * *)result_2(D)], prephitmp_29
	movq	16(%rax), %r9	 # _63->mSize, _174
 # data.hpp:152:         assertm(i >= 0 && i < size(), "Index out of bounds!");
	cmpq	%r9, %rbx	 # _174, k
	jnb	.L256	 #,
 # array.hpp:333:         for (size_t k = 0; k < mFlatLength; k++)
	movslq	16(%rsi), %rax	 # result_2(D)->mFlatLength, result_2(D)->mFlatLength
 # array.hpp:333:         for (size_t k = 0; k < mFlatLength; k++)
	addq	$1, %rbx	 #, k
 # array.hpp:334:             mData[k] = swap ? f(other.mData[k], mData[k]) : f(mData[k], other.mData[k]);
	movsd	%xmm0, (%r10,%r12)	 # _50, *_177
 # array.hpp:333:         for (size_t k = 0; k < mFlatLength; k++)
	cmpq	%rax, %rbx	 # result_2(D)->mFlatLength, k
	jb	.L136	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/std_function.h:243:       if (_M_manager)
	movq	80(%rsp), %r13	 # MEM[(struct _Function_base *)&D.119191]._M_manager, _9
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/std_function.h:243:       if (_M_manager)
	testq	%r13, %r13	 # _9
	je	.L108	 #,
.L130:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/std_function.h:244: 	_M_manager(_M_functor, _M_functor, __destroy_functor);
	movl	$3, %r8d	 #,
	movq	%rbp, %rdx	 # tmp215,
	movq	%rbp, %rcx	 # tmp215,
	call	*%r13	 # _9
	nop	
.L108:
 # array.hpp:512:     }
	movaps	96(%rsp), %xmm6	 #,
	movq	%rsi, %rax	 # <retval>,
	addq	$120, %rsp	 #,
	popq	%rbx	 #
	popq	%rsi	 #
	popq	%rdi	 #
	popq	%rbp	 #
	popq	%r12	 #
	popq	%r13	 #
	popq	%r14	 #
	popq	%r15	 #
	ret	
	.p2align 4,,10
	.p2align 3
.L246:
 # array.hpp:427:                 if (shape1[i] == 1)
	cmpl	$1, %ecx	 #, _114
	je	.L119	 #,
 # array.hpp:429:                 else if (shape2[i] == 1)
	cmpl	$1, %r8d	 #, _221
	je	.L119	 #,
.L120:
 # array.hpp:329:         assertm(isShapeMatch(mShape, other.mShape), "The shape of the other array is not the same as the shape of this array.");
	movl	$329, %r8d	 #,
	leaq	.LC6(%rip), %rdx	 #, tmp187
	leaq	.LC7(%rip), %rcx	 #, tmp188
.LEHB12:
	call	*__imp__assert(%rip)	 #
	.p2align 4,,10
	.p2align 3
.L247:
 # array.hpp:427:                 if (shape1[i] == 1)
	cmpl	$1, %ecx	 #, _200
	je	.L121	 #,
 # array.hpp:429:                 else if (shape2[i] == 1)
	cmpl	$1, %r8d	 #, _184
	je	.L121	 #,
	jmp	.L120	 #
	.p2align 4,,10
	.p2align 3
.L248:
 # array.hpp:427:                 if (shape1[i] == 1)
	cmpl	$1, %ecx	 #, _117
	je	.L122	 #,
 # array.hpp:429:                 else if (shape2[i] == 1)
	cmpl	$1, %r8d	 #, _183
	je	.L122	 #,
	jmp	.L120	 #
	.p2align 4,,10
	.p2align 3
.L249:
 # array.hpp:427:                 if (shape1[i] == 1)
	cmpl	$1, %ecx	 #, _210
	je	.L123	 #,
 # array.hpp:429:                 else if (shape2[i] == 1)
	cmpl	$1, %r8d	 #, _76
	je	.L123	 #,
	jmp	.L120	 #
	.p2align 4,,10
	.p2align 3
.L250:
 # array.hpp:427:                 if (shape1[i] == 1)
	cmpl	$1, %ecx	 #, _108
	je	.L124	 #,
 # array.hpp:429:                 else if (shape2[i] == 1)
	cmpl	$1, %r8d	 #, _87
	je	.L124	 #,
	jmp	.L120	 #
	.p2align 4,,10
	.p2align 3
.L251:
 # array.hpp:427:                 if (shape1[i] == 1)
	cmpl	$1, %ecx	 #, _145
	je	.L125	 #,
 # array.hpp:429:                 else if (shape2[i] == 1)
	cmpl	$1, %r8d	 #, _52
	je	.L125	 #,
	jmp	.L120	 #
	.p2align 4,,10
	.p2align 3
.L252:
 # array.hpp:427:                 if (shape1[i] == 1)
	cmpl	$1, %ecx	 #, _101
	je	.L126	 #,
 # array.hpp:429:                 else if (shape2[i] == 1)
	cmpl	$1, %r8d	 #, _246
	je	.L126	 #,
	jmp	.L120	 #
	.p2align 4,,10
	.p2align 3
.L253:
 # array.hpp:427:                 if (shape1[i] == 1)
	cmpl	$1, %ecx	 #, _53
	je	.L128	 #,
 # array.hpp:429:                 else if (shape2[i] == 1)
	cmpl	$1, %eax	 #, _122
	je	.L128	 #,
	jmp	.L120	 #
	.p2align 4,,10
	.p2align 3
.L110:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/stl_algobase.h:438: 	  else if (_Num == 1)
	jne	.L111	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/stl_algobase.h:398: 	{ *__to = *__from; }
	movsd	(%rdx), %xmm0	 # *_92, _102
	movsd	%xmm0, (%r10)	 # _102, *_91
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/stl_algobase.h:398: 	{ *__to = *__from; }
	jmp	.L111	 #
.L140:
	leaq	64(%rsp), %rbp	 #, tmp215
	jmp	.L130	 #
.L256:
 # data.hpp:152:         assertm(i >= 0 && i < size(), "Index out of bounds!");
	movl	$152, %r8d	 #,
	leaq	.LC0(%rip), %rdx	 #, tmp202
	leaq	.LC5(%rip), %rcx	 #, tmp203
	call	*__imp__assert(%rip)	 #
.L254:
	movl	$152, %r8d	 #,
	leaq	.LC0(%rip), %rdx	 #, tmp194
	leaq	.LC5(%rip), %rcx	 #, tmp195
	call	*__imp__assert(%rip)	 #
.L132:
	movl	$152, %r8d	 #,
	leaq	.LC0(%rip), %rdx	 #, tmp191
	leaq	.LC5(%rip), %rcx	 #, tmp192
	call	*__imp__assert(%rip)	 #
.L255:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/std_function.h:590: 	  __throw_bad_function_call();
	call	_ZSt25__throw_bad_function_callv	 #
.LEHE12:
.L141:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/std_function.h:243:       if (_M_manager)
	movq	%rax, %rbx	 # tmp229, tmp178
	jmp	.L118	 #
.L115:
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	movl	$16, %ecx	 #,
	call	__cxa_allocate_exception	 #
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	leaq	.LC4(%rip), %rdx	 #, tmp183
	movq	%rax, %rcx	 # _140,
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	movq	%rax, %rdi	 # tmp226, _140
.LEHB13:
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	call	_ZNSt12out_of_rangeC1EPKc	 #
.LEHE13:
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	leaq	_ZNSt12out_of_rangeD1Ev(%rip), %r8	 #,
	leaq	_ZTISt12out_of_range(%rip), %rdx	 #, tmp184
	movq	%rdi, %rcx	 # _140,
.LEHB14:
	call	__cxa_throw	 #
.LEHE14:
.L129:
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	movq	%rdi, %rcx	 # _140,
	call	__cxa_free_exception	 #
.L118:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/std_function.h:243:       if (_M_manager)
	movq	80(%rsp), %rax	 # MEM[(struct _Function_base *)&D.119191]._M_manager, _10
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/std_function.h:243:       if (_M_manager)
	testq	%rax, %rax	 # _10
	je	.L138	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/std_function.h:244: 	_M_manager(_M_functor, _M_functor, __destroy_functor);
	leaq	64(%rsp), %rcx	 #, tmp210
	movl	$3, %r8d	 #,
	movq	%rcx, %rdx	 # tmp210,
	call	*%rax	 # _10
.L138:
 # data.hpp:167:         if (mControl != nullptr)
	cmpq	$0, 8(%rsi)	 #, MEM[(struct Data *)result_2(D)].mControl
	je	.L139	 #,
 # data.hpp:168:             release();
	movq	%rsi, %rcx	 # <retval>,
	call	_ZN4DataIdE7releaseEv	 #
.L139:
	movq	%rbx, %rcx	 # tmp178,
.LEHB15:
	call	_Unwind_Resume	 #
.LEHE15:
.L143:
.L245:
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	movq	%rax, %rbx	 # tmp227, tmp185
	jmp	.L129	 #
.L114:
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	movl	$16, %ecx	 #,
	call	__cxa_allocate_exception	 #
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	leaq	.LC4(%rip), %rdx	 #, tmp176
	movq	%rax, %rcx	 # _137,
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	movq	%rax, %rdi	 # tmp224, _137
.LEHB16:
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	call	_ZNSt12out_of_rangeC1EPKc	 #
.LEHE16:
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	leaq	_ZNSt12out_of_rangeD1Ev(%rip), %r8	 #,
	leaq	_ZTISt12out_of_range(%rip), %rdx	 #, tmp177
	movq	%rdi, %rcx	 # _137,
.LEHB17:
	call	__cxa_throw	 #
.LEHE17:
.L109:
.LEHB18:
 # data.hpp:146:         T *newRaw = new T[s];
	call	__cxa_throw_bad_array_new_length	 #
.LEHE18:
.L142:
	jmp	.L245	 #
	.seh_handler	__gxx_personality_seh0, @unwind, @except
	.seh_handlerdata
.LLSDA6090:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE6090-.LLSDACSB6090
.LLSDACSB6090:
	.uleb128 .LEHB10-.LFB6090
	.uleb128 .LEHE10-.LEHB10
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB11-.LFB6090
	.uleb128 .LEHE11-.LEHB11
	.uleb128 .L141-.LFB6090
	.uleb128 0
	.uleb128 .LEHB12-.LFB6090
	.uleb128 .LEHE12-.LEHB12
	.uleb128 .L141-.LFB6090
	.uleb128 0
	.uleb128 .LEHB13-.LFB6090
	.uleb128 .LEHE13-.LEHB13
	.uleb128 .L143-.LFB6090
	.uleb128 0
	.uleb128 .LEHB14-.LFB6090
	.uleb128 .LEHE14-.LEHB14
	.uleb128 .L141-.LFB6090
	.uleb128 0
	.uleb128 .LEHB15-.LFB6090
	.uleb128 .LEHE15-.LEHB15
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB16-.LFB6090
	.uleb128 .LEHE16-.LEHB16
	.uleb128 .L142-.LFB6090
	.uleb128 0
	.uleb128 .LEHB17-.LFB6090
	.uleb128 .LEHE17-.LEHB17
	.uleb128 .L141-.LFB6090
	.uleb128 0
	.uleb128 .LEHB18-.LFB6090
	.uleb128 .LEHE18-.LEHB18
	.uleb128 0
	.uleb128 0
.LLSDACSE6090:
	.section	.text$_ZNK5ArrayIdEmlERKS0_,"x"
	.linkonce discard
	.seh_endproc
	.section	.text$_ZN20RandomArrayGenerator14uniformIntegerIdEE5ArrayIT_ERK11StackBufferIlLy8EEll,"x"
	.linkonce discard
	.align 2
	.p2align 4
	.globl	_ZN20RandomArrayGenerator14uniformIntegerIdEE5ArrayIT_ERK11StackBufferIlLy8EEll
	.def	_ZN20RandomArrayGenerator14uniformIntegerIdEE5ArrayIT_ERK11StackBufferIlLy8EEll;	.scl	2;	.type	32;	.endef
	.seh_proc	_ZN20RandomArrayGenerator14uniformIntegerIdEE5ArrayIT_ERK11StackBufferIlLy8EEll
_ZN20RandomArrayGenerator14uniformIntegerIdEE5ArrayIT_ERK11StackBufferIlLy8EEll:
.LFB6085:
	pushq	%r15	 #
	.seh_pushreg	%r15
	pushq	%r14	 #
	.seh_pushreg	%r14
	pushq	%r13	 #
	.seh_pushreg	%r13
	pushq	%r12	 #
	.seh_pushreg	%r12
	pushq	%rbp	 #
	.seh_pushreg	%rbp
	pushq	%rdi	 #
	.seh_pushreg	%rdi
	pushq	%rsi	 #
	.seh_pushreg	%rsi
	pushq	%rbx	 #
	.seh_pushreg	%rbx
	subq	$72, %rsp	 #,
	.seh_stackalloc	72
	.seh_endprologue
 # random.hpp:55:             low = 0;
	xorl	%r12d, %r12d	 # low
 # random.hpp:50:     Array<T> uniformInteger(const Coordinates &shape, long low = 0, long high = 2)
	movl	176(%rsp), %eax	 # high, high
	movq	%r8, 160(%rsp)	 # tmp347, shape
	movq	%rcx, %rbp	 # tmp345, <retval>
	movq	%rdx, %rbx	 # tmp346, this
	movl	%r9d, %r13d	 # tmp348, low
 # random.hpp:52:         if (low >= high)
	cmpl	%eax, %r9d	 # high, low
	jge	.L258	 #,
	movl	%r9d, %r12d	 # low, low
	movl	%eax, %r13d	 # high, low
.L258:
 # stack_buffer.hpp:19:     size_t size() const { return mSize; }
	movq	160(%rsp), %rax	 # shape, tmp400
	movq	32(%rax), %rax	 # MEM[(const struct StackBuffer *)shape_14(D)].mSize, _233
 # array.hpp:37:         for (long i = 0; i < shape.size(); i++)
	testq	%rax, %rax	 # _233
	je	.L259	 #,
 # array.hpp:39:             flatLength *= shape[i];
	movq	160(%rsp), %rdi	 # shape, tmp401
	movl	(%rdi), %r15d	 # MEM <const struct StackBuffer> [(const long int &)shape_14(D)].mBuffer[0], flatLength
 # array.hpp:37:         for (long i = 0; i < shape.size(); i++)
	cmpq	$1, %rax	 #, _233
	je	.L260	 #,
 # array.hpp:39:             flatLength *= shape[i];
	imull	4(%rdi), %r15d	 # MEM <const struct StackBuffer> [(const long int &)shape_14(D)].mBuffer[1], flatLength
 # array.hpp:37:         for (long i = 0; i < shape.size(); i++)
	cmpq	$2, %rax	 #, _233
	je	.L260	 #,
 # array.hpp:39:             flatLength *= shape[i];
	imull	8(%rdi), %r15d	 # MEM <const struct StackBuffer> [(const long int &)shape_14(D)].mBuffer[2], flatLength
 # array.hpp:37:         for (long i = 0; i < shape.size(); i++)
	cmpq	$3, %rax	 #, _233
	je	.L260	 #,
 # array.hpp:39:             flatLength *= shape[i];
	imull	12(%rdi), %r15d	 # MEM <const struct StackBuffer> [(const long int &)shape_14(D)].mBuffer[3], flatLength
 # array.hpp:37:         for (long i = 0; i < shape.size(); i++)
	cmpq	$4, %rax	 #, _233
	je	.L260	 #,
 # array.hpp:39:             flatLength *= shape[i];
	imull	16(%rdi), %r15d	 # MEM <const struct StackBuffer> [(const long int &)shape_14(D)].mBuffer[4], flatLength
 # array.hpp:37:         for (long i = 0; i < shape.size(); i++)
	cmpq	$5, %rax	 #, _233
	je	.L260	 #,
 # array.hpp:39:             flatLength *= shape[i];
	imull	20(%rdi), %r15d	 # MEM <const struct StackBuffer> [(const long int &)shape_14(D)].mBuffer[5], flatLength
 # array.hpp:37:         for (long i = 0; i < shape.size(); i++)
	cmpq	$6, %rax	 #, _233
	je	.L260	 #,
 # array.hpp:39:             flatLength *= shape[i];
	imull	24(%rdi), %r15d	 # MEM <const struct StackBuffer> [(const long int &)shape_14(D)].mBuffer[6], flatLength
 # array.hpp:37:         for (long i = 0; i < shape.size(); i++)
	cmpq	$7, %rax	 #, _233
	je	.L260	 #,
 # array.hpp:39:             flatLength *= shape[i];
	imull	28(%rdi), %r15d	 # MEM <const struct StackBuffer> [(const long int &)shape_14(D)].mBuffer[7], flatLength
.L260:
 # random.hpp:59:         auto data = Data<T>(Array<T>::calculateFlatLength(shape));
	movslq	%r15d, %rsi	 # flatLength, _1
 # data.hpp:70:         mRaw = new T[size];
	movq	%rsi, %rax	 # _1, tmp364
	shrq	$60, %rax	 #, tmp364
	jne	.L262	 #,
 # data.hpp:70:         mRaw = new T[size];
	leaq	0(,%rsi,8), %rcx	 #, tmp210
.LEHB19:
 # data.hpp:70:         mRaw = new T[size];
	call	_Znay	 #
 # data.hpp:71:         mControl = new Control(mRaw, size);
	movl	$32, %ecx	 #,
 # data.hpp:70:         mRaw = new T[size];
	movq	%rax, %r14	 # tmp351, _349
 # data.hpp:70:         mRaw = new T[size];
	movq	%rax, 48(%rsp)	 # _349, data.mRaw
 # data.hpp:71:         mControl = new Control(mRaw, size);
	call	_Znwy	 #
.LEHE19:
 # data.hpp:39:             mRaw = raw;
	movq	%r14, (%rax)	 # _349, MEM[(struct Control *)_55].mRaw
 # data.hpp:71:         mControl = new Control(mRaw, size);
	movq	%rax, %r10	 # tmp352, _347
 # data.hpp:40:             mSize = size;
	movq	%rsi, 16(%rax)	 # _1, MEM[(struct Control *)_55].mSize
 # data.hpp:41:             mAccessCount = 1;
	movq	$1, 8(%rax)	 #, MEM[(struct Control *)_55].mAccessCount
 # data.hpp:42:             mRealeased = false;
	movb	$0, 24(%rax)	 #, MEM[(struct Control *)_55].mRealeased
 # data.hpp:71:         mControl = new Control(mRaw, size);
	movq	%rax, 56(%rsp)	 # _347, data.mControl
 # random.hpp:60:         for (size_t i = 0; i < data.size(); i++)
	testq	%rsi, %rsi	 # _1
	je	.L358	 #,
.L261:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:290: 	const __uctype __urange
	subl	%r12d, %r13d	 # low, __urange
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:323: 		const __uctype __scaling = __urngrange / __uerange;
	movq	%r10, 40(%rsp)	 # _347, %sfp
 # random.hpp:60:         for (size_t i = 0; i < data.size(); i++)
	xorl	%r11d, %r11d	 # i
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:353: 			 (__urng, param_type(0, __urange / __uerngrange)));
	movl	%r13d, %eax	 # __urange, tmp215
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:323: 		const __uctype __scaling = __urngrange / __uerange;
	movq	%rbp, 144(%rsp)	 # <retval>, .result_ptr
	movq	%rsi, %rbp	 # _1, _1
	movq	%r11, %rsi	 # i, i
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:353: 			 (__urng, param_type(0, __urange / __uerngrange)));
	shrl	%eax	 #
	movq	%rax, %rdx	 # tmp216, tmp218
	salq	$30, %rdx	 #, tmp218
	addq	%rax, %rdx	 # tmp216, tmp219
	leaq	(%rax,%rdx,2), %rdi	 #, tmp221
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:298: 	    const __uctype __uerange = __urange + 1; // __urange can be zero
	leal	1(%r13), %eax	 #, __uerange
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:353: 			 (__urng, param_type(0, __urange / __uerngrange)));
	shrq	$61, %rdi	 #, _84
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:323: 		const __uctype __scaling = __urngrange / __uerange;
	movl	%eax, %r11d	 # __uerange, __uerange
	.p2align 4,,10
	.p2align 3
.L279:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:294: 	if (__urngrange > __urange)
	cmpl	$2147483644, %r13d	 #, __urange
	ja	.L265	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:323: 		const __uctype __scaling = __urngrange / __uerange;
	movl	$2147483645, %eax	 #, tmp223
	xorl	%edx, %edx	 # tmp224
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:324: 		const __uctype __past = __uerange * __scaling;
	movl	%r11d, %r9d	 # __uerange, __past
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:323: 		const __uctype __scaling = __urngrange / __uerange;
	divl	%r11d	 # __uerange
	movl	(%rbx), %edx	 # MEM[(struct linear_congruential_engine *)this_21(D)]._M_x, this___M_x_lsm.322
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:324: 		const __uctype __past = __uerange * __scaling;
	imull	%eax, %r9d	 # tmp223, __past
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:323: 		const __uctype __scaling = __urngrange / __uerange;
	movl	%eax, %ecx	 # tmp223, tmp223
	.p2align 4,,10
	.p2align 3
.L269:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:61: 	    _Tp __t1 = __a * (__x % __q);
	movl	%edx, %eax	 # this___M_x_lsm.322, this___M_x_lsm.322
	movl	%edx, %r8d	 # this___M_x_lsm.322, tmp233
	imulq	$110892733, %rax, %rax	 #, this___M_x_lsm.322, tmp231
	shrq	$32, %rax	 #, tmp232
	subl	%eax, %r8d	 # tmp232, tmp233
	shrl	%r8d	 # tmp234
	addl	%r8d, %eax	 # tmp234, tmp235
	shrl	$16, %eax	 #, tmp229
	imull	$127773, %eax, %r8d	 #, tmp229, tmp236
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:62: 	    _Tp __t2 = __r * (__x / __q);
	imull	$2836, %eax, %eax	 #, tmp229, __t2
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:61: 	    _Tp __t1 = __a * (__x % __q);
	subl	%r8d, %edx	 # tmp236, tmp237
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:61: 	    _Tp __t1 = __a * (__x % __q);
	imull	$16807, %edx, %edx	 #, tmp237, __t1
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:63: 	    if (__t1 >= __t2)
	cmpl	%eax, %edx	 # __t2, __t1
	jnb	.L356	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:66: 	      __x = __m - __t2 + __t1;
	addl	$2147483647, %edx	 #, tmp245
.L356:
	subl	%eax, %edx	 # __t2, this___M_x_lsm.322
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:326: 		  __ret = __uctype(__urng()) - __urngmin;
	leal	-1(%rdx), %eax	 #, __ret
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:327: 		while (__ret >= __past);
	cmpl	%r9d, %eax	 # __past, __ret
	jnb	.L269	 #,
	movl	%edx, (%rbx)	 # this___M_x_lsm.322, MEM[(struct linear_congruential_engine *)this_21(D)]._M_x
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:328: 		__ret /= __scaling;
	xorl	%edx, %edx	 # tmp247
	divl	%ecx	 # tmp223
.L270:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:361: 	return __ret + __param.a();
	addl	%r12d, %eax	 # low, tmp292
 # random.hpp:61:             data[i] = distribution(mRandomEngine);
	pxor	%xmm0, %xmm0	 # _3
	cvtsi2sdl	%eax, %xmm0	 # tmp292, _3
 # data.hpp:152:         assertm(i >= 0 && i < size(), "Index out of bounds!");
	cmpq	%rbp, %rsi	 # _1, i
	je	.L359	 #,
 # random.hpp:61:             data[i] = distribution(mRandomEngine);
	movsd	%xmm0, (%r14,%rsi,8)	 # _3, MEM[(double &)_349 + i_230 * 8]
 # random.hpp:60:         for (size_t i = 0; i < data.size(); i++)
	addq	$1, %rsi	 #, i
 # random.hpp:60:         for (size_t i = 0; i < data.size(); i++)
	cmpq	%rbp, %rsi	 # _1, i
	jne	.L279	 #,
	movq	40(%rsp), %r10	 # %sfp, _347
	movq	144(%rsp), %rbp	 # .result_ptr, <retval>
.L280:
 # stack_buffer.hpp:74:         mSize = other.mSize;
	movq	160(%rsp), %rax	 # shape, tmp410
 # data.hpp:100:         mControl = other.mControl;
	movq	%r10, 8(%rbp)	 # _347, MEM[(struct Data *)_17(D)].mControl
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/stl_algobase.h:437: 	    __builtin_memmove(__result, __first, sizeof(_Tp) * _Num);
	leaq	24(%rbp), %rcx	 #, tmp214
	movl	$32, %r8d	 #,
 # data.hpp:101:         mControl->mAccessCount++;
	movq	$2, 8(%r10)	 #, MEM[(struct Control *)_348].mAccessCount
 # stack_buffer.hpp:74:         mSize = other.mSize;
	movq	32(%rax), %rbx	 # MEM[(const struct StackBuffer &)shape_14(D)].mSize, _125
 # data.hpp:99:         mRaw = other.mRaw;
	movq	%r14, 0(%rbp)	 # _349, MEM[(struct Data *)_17(D)].mRaw
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/stl_algobase.h:437: 	    __builtin_memmove(__result, __first, sizeof(_Tp) * _Num);
	movq	%rax, %rdx	 # tmp410,
 # array.hpp:83:     Array<T>(const Data<T> &data, const Coordinates &shape) : mData(data), mFlatLength(mData.size()), mShape(shape), mDim(shape.size()), mStrides(shape.size())
	movl	%r15d, 16(%rbp)	 # flatLength, *_17(D).mFlatLength
 # stack_buffer.hpp:74:         mSize = other.mSize;
	movq	%rbx, 56(%rbp)	 # _125, MEM[(struct StackBuffer *)_17(D) + 24B].mSize
 # data.hpp:101:         mControl->mAccessCount++;
	movq	%r10, 40(%rsp)	 # _347, %sfp
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/stl_algobase.h:437: 	    __builtin_memmove(__result, __first, sizeof(_Tp) * _Num);
	call	memmove	 #
 # stack_buffer.hpp:19:     size_t size() const { return mSize; }
	movq	160(%rsp), %rax	 # shape, tmp411
 # stack_buffer.hpp:28:         if (size > BUFFER_SIZE)
	movq	40(%rsp), %r10	 # %sfp, _347
 # stack_buffer.hpp:19:     size_t size() const { return mSize; }
	movq	32(%rax), %rax	 # MEM[(const struct StackBuffer *)shape_14(D)].mSize, _129
 # stack_buffer.hpp:28:         if (size > BUFFER_SIZE)
	cmpq	$8, %rax	 #, _129
	ja	.L360	 #,
 # stack_buffer.hpp:30:         mSize = size;
	movq	%rax, 96(%rbp)	 # _129, MEM[(struct StackBuffer *)_17(D) + 64B].mSize
 # array.hpp:83:     Array<T>(const Data<T> &data, const Coordinates &shape) : mData(data), mFlatLength(mData.size()), mShape(shape), mDim(shape.size()), mStrides(shape.size())
	movb	$0, 104(%rbp)	 #, *_17(D).transposed
 # array.hpp:83:     Array<T>(const Data<T> &data, const Coordinates &shape) : mData(data), mFlatLength(mData.size()), mShape(shape), mDim(shape.size()), mStrides(shape.size())
	movl	%eax, 108(%rbp)	 # _129, *_17(D).mDim
 # array.hpp:179:         for (size_t i = 0; i < mDim; i++)
	testq	%rax, %rax	 # _129
	je	.L282	 #,
	movq	%rax, %rcx	 # _129, tmp332
	addq	$-1, %rcx	 #, tmp332
	jc	.L361	 #,
.L288:
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	movl	$16, %ecx	 #,
	call	__cxa_allocate_exception	 #
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	leaq	.LC4(%rip), %rdx	 #, tmp304
	movq	%rax, %rcx	 # _155,
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	movq	%rax, %rsi	 # tmp356, _155
.LEHB20:
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	call	_ZNSt12out_of_rangeC1EPKc	 #
.LEHE20:
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	leaq	_ZNSt12out_of_rangeD1Ev(%rip), %r8	 #,
	leaq	_ZTISt12out_of_range(%rip), %rdx	 #, tmp305
	movq	%rsi, %rcx	 # _155,
.LEHB21:
	call	__cxa_throw	 #
.LEHE21:
	.p2align 4,,10
	.p2align 3
.L265:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:331: 	else if (__urngrange < __urange)
	cmpl	$2147483645, %r13d	 #, __urange
	je	.L271	 #,
.L275:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:353: 			 (__urng, param_type(0, __urange / __uerngrange)));
	movl	%edi, %r8d	 # _84,
	xorl	%edx, %edx	 #
	movq	%rbx, %rcx	 # this,
	call	_ZNSt24uniform_int_distributionIlEclISt26linear_congruential_engineIjLj16807ELj0ELj2147483647EEEElRT_RKNS0_10param_typeE.isra.0	 #
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.h:367: 	_M_x = __detail::__mod<_UIntType, __m, __a, __c>(_M_x);
	movl	(%rbx), %r8d	 # MEM[(struct linear_congruential_engine *)this_21(D)]._M_x,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:352: 		__tmp = (__uerngrange * operator()
	imull	$2147483646, %eax, %edx	 #, tmp353, __tmp
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.h:367: 	_M_x = __detail::__mod<_UIntType, __m, __a, __c>(_M_x);
	movq	%r8, %rax	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:61: 	    _Tp __t1 = __a * (__x % __q);
	imulq	$110892733, %r8, %r8	 #, _88, tmp250
	movl	%eax, %ecx	 # _88, tmp252
	shrq	$32, %r8	 #, tmp251
	subl	%r8d, %ecx	 # tmp251, tmp252
	shrl	%ecx	 # tmp253
	addl	%r8d, %ecx	 # tmp251, tmp254
	shrl	$16, %ecx	 #, tmp248
	imull	$127773, %ecx, %r8d	 #, tmp248, tmp255
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:62: 	    _Tp __t2 = __r * (__x / __q);
	imull	$2836, %ecx, %ecx	 #, tmp248, __t2
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:61: 	    _Tp __t1 = __a * (__x % __q);
	subl	%r8d, %eax	 # tmp255, tmp256
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:61: 	    _Tp __t1 = __a * (__x % __q);
	imull	$16807, %eax, %eax	 #, tmp256, __t1
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:63: 	    if (__t1 >= __t2)
	cmpl	%ecx, %eax	 # __t2, __t1
	jb	.L272	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:64: 	      __x = __t1 - __t2;
	subl	%ecx, %eax	 # __t2, __x
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.h:367: 	_M_x = __detail::__mod<_UIntType, __m, __a, __c>(_M_x);
	movl	%eax, (%rbx)	 # __x, MEM[(struct linear_congruential_engine *)this_21(D)]._M_x
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:354: 		__ret = __tmp + (__uctype(__urng()) - __urngmin);
	leal	-1(%rdx,%rax), %eax	 #, __ret
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:356: 	    while (__ret > __urange || __ret < __tmp);
	cmpl	%edx, %eax	 # __tmp, __ret
	jb	.L275	 #,
	cmpl	%eax, %r13d	 # __ret, __urange
	jb	.L275	 #,
	jmp	.L270	 #
	.p2align 4,,10
	.p2align 3
.L272:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:66: 	      __x = __m - __t2 + __t1;
	addl	$2147483647, %eax	 #, tmp269
	subl	%ecx, %eax	 # __t2, __x
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.h:367: 	_M_x = __detail::__mod<_UIntType, __m, __a, __c>(_M_x);
	movl	%eax, (%rbx)	 # __x, MEM[(struct linear_congruential_engine *)this_21(D)]._M_x
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:354: 		__ret = __tmp + (__uctype(__urng()) - __urngmin);
	leal	-1(%rdx,%rax), %eax	 #, __ret
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:356: 	    while (__ret > __urange || __ret < __tmp);
	cmpl	%eax, %r13d	 # __ret, __urange
	jb	.L275	 #,
	cmpl	%edx, %eax	 # __tmp, __ret
	jb	.L275	 #,
	jmp	.L270	 #
	.p2align 4,,10
	.p2align 3
.L271:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.h:367: 	_M_x = __detail::__mod<_UIntType, __m, __a, __c>(_M_x);
	movl	(%rbx), %eax	 # MEM[(struct linear_congruential_engine *)this_21(D)]._M_x,
	movq	%rax, %rcx	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:61: 	    _Tp __t1 = __a * (__x % __q);
	imulq	$110892733, %rax, %rax	 #, _102, tmp277
	movl	%ecx, %edx	 # _102, tmp279
	shrq	$32, %rax	 #, tmp278
	subl	%eax, %edx	 # tmp278, tmp279
	shrl	%edx	 # tmp280
	addl	%eax, %edx	 # tmp278, tmp281
	shrl	$16, %edx	 #, tmp275
	imull	$127773, %edx, %eax	 #, tmp275, tmp282
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:62: 	    _Tp __t2 = __r * (__x / __q);
	imull	$2836, %edx, %edx	 #, tmp275, __t2
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:61: 	    _Tp __t1 = __a * (__x % __q);
	subl	%eax, %ecx	 # tmp282, tmp283
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:61: 	    _Tp __t1 = __a * (__x % __q);
	imull	$16807, %ecx, %ecx	 #, tmp283, __t1
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:64: 	      __x = __t1 - __t2;
	movl	%ecx, %r9d	 # __t1, tmp338
	leal	2147483647(%rcx), %eax	 #, tmp291
	subl	%edx, %r9d	 # __t2, tmp338
	subl	%edx, %eax	 # __t2, tmp339
	cmpl	%edx, %ecx	 # __t2, __t1
	cmovnb	%r9d, %eax	 # tmp339,, tmp338, __x
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.h:367: 	_M_x = __detail::__mod<_UIntType, __m, __a, __c>(_M_x);
	movl	%eax, (%rbx)	 # __x, MEM[(struct linear_congruential_engine *)this_21(D)]._M_x
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/uniform_int_dist.h:359: 	  __ret = __uctype(__urng()) - __urngmin;
	subl	$1, %eax	 #, __ret
	jmp	.L270	 #
.L361:
	leaq	0(%rbp,%rax,4), %rdx	 #, _62
 # array.hpp:181:             mStrides[mDim - i - 1] = multiplier;
	movl	$1, 60(%rdx)	 #, MEM[(long int &)_62 + 60]
 # stack_buffer.hpp:125:         if (i >= mSize)
	cmpq	%rbx, %rcx	 # _125, tmp332
	jnb	.L289	 #,
 # array.hpp:182:             multiplier *= mShape[mDim - i - 1];
	movslq	20(%rdx), %r8	 # MEM[(long int &)_62 + 20],
 # array.hpp:179:         for (size_t i = 0; i < mDim; i++)
	cmpq	$1, %rax	 #, _129
	je	.L282	 #,
	movq	%rax, %r9	 # _129, tmp311
	addq	$-2, %r9	 #, tmp311
	jnc	.L288	 #,
 # array.hpp:181:             mStrides[mDim - i - 1] = multiplier;
	movl	%r8d, 56(%rdx)	 # _73, MEM[(long int &)_62 + 56]
 # stack_buffer.hpp:125:         if (i >= mSize)
	cmpq	%rbx, %r9	 # _125, tmp311
	jnb	.L289	 #,
 # array.hpp:182:             multiplier *= mShape[mDim - i - 1];
	movslq	16(%rdx), %rcx	 # MEM[(long int &)_62 + 16], MEM[(long int &)_62 + 16]
	imulq	%r8, %rcx	 # _74, multiplier
 # array.hpp:179:         for (size_t i = 0; i < mDim; i++)
	cmpq	$2, %rax	 #, _129
	je	.L282	 #,
	movq	%rax, %r8	 # _129, tmp314
	addq	$-3, %r8	 #, tmp314
	jnc	.L288	 #,
 # array.hpp:181:             mStrides[mDim - i - 1] = multiplier;
	movl	%ecx, 52(%rdx)	 # multiplier, MEM[(long int &)_62 + 52]
 # stack_buffer.hpp:125:         if (i >= mSize)
	cmpq	%rbx, %r8	 # _125, tmp314
	jnb	.L289	 #,
 # array.hpp:182:             multiplier *= mShape[mDim - i - 1];
	movslq	12(%rdx), %r8	 # MEM[(long int &)_62 + 12], MEM[(long int &)_62 + 12]
	imulq	%rcx, %r8	 # multiplier, multiplier
 # array.hpp:179:         for (size_t i = 0; i < mDim; i++)
	cmpq	$3, %rax	 #, _129
	je	.L282	 #,
	movq	%rax, %rcx	 # _129, tmp317
	addq	$-4, %rcx	 #, tmp317
	jnc	.L288	 #,
 # array.hpp:181:             mStrides[mDim - i - 1] = multiplier;
	movl	%r8d, 48(%rdx)	 # multiplier, MEM[(long int &)_62 + 48]
 # stack_buffer.hpp:125:         if (i >= mSize)
	cmpq	%rbx, %rcx	 # _125, tmp317
	jnb	.L289	 #,
 # array.hpp:182:             multiplier *= mShape[mDim - i - 1];
	movslq	8(%rdx), %rcx	 # MEM[(long int &)_62 + 8], MEM[(long int &)_62 + 8]
	imulq	%r8, %rcx	 # multiplier, multiplier
 # array.hpp:179:         for (size_t i = 0; i < mDim; i++)
	cmpq	$4, %rax	 #, _129
	je	.L282	 #,
 # array.hpp:181:             mStrides[mDim - i - 1] = multiplier;
	leaq	-5(%rax), %r8	 #, tmp320
	movl	%ecx, 44(%rdx)	 # multiplier, MEM[(long int &)_62 + 44]
 # stack_buffer.hpp:125:         if (i >= mSize)
	cmpq	%rbx, %r8	 # _125, tmp320
	jnb	.L289	 #,
 # array.hpp:182:             multiplier *= mShape[mDim - i - 1];
	movslq	4(%rdx), %r8	 # MEM[(long int &)_62 + 4], MEM[(long int &)_62 + 4]
	imulq	%r8, %rcx	 # MEM[(long int &)_62 + 4], multiplier
 # array.hpp:179:         for (size_t i = 0; i < mDim; i++)
	cmpq	$5, %rax	 #, _129
	je	.L282	 #,
 # array.hpp:181:             mStrides[mDim - i - 1] = multiplier;
	leaq	-6(%rax), %r8	 #, tmp323
	movl	%ecx, 40(%rdx)	 # multiplier, MEM[(long int &)_62 + 40]
 # stack_buffer.hpp:125:         if (i >= mSize)
	cmpq	%rbx, %r8	 # _125, tmp323
	jnb	.L289	 #,
 # array.hpp:182:             multiplier *= mShape[mDim - i - 1];
	movslq	(%rdx), %r8	 # *_62, *_62
	imulq	%r8, %rcx	 # *_62, multiplier
 # array.hpp:179:         for (size_t i = 0; i < mDim; i++)
	cmpq	$6, %rax	 #, _129
	je	.L282	 #,
 # array.hpp:181:             mStrides[mDim - i - 1] = multiplier;
	xorl	%r8d, %r8d	 # tmp326
	cmpq	$7, %rax	 #, _129
	movl	%ecx, 36(%rdx)	 # multiplier, MEM[(long int &)_62 + 36]
	setne	%r8b	 #, tmp326
 # stack_buffer.hpp:125:         if (i >= mSize)
	cmpq	%rbx, %r8	 # _125, tmp326
	jnb	.L289	 #,
 # array.hpp:182:             multiplier *= mShape[mDim - i - 1];
	movslq	-4(%rdx), %r8	 # MEM[(long int &)_62 + 18446744073709551612], MEM[(long int &)_62 + 18446744073709551612]
	imulq	%rcx, %r8	 # multiplier, multiplier
 # array.hpp:179:         for (size_t i = 0; i < mDim; i++)
	cmpq	$8, %rax	 #, _129
	jne	.L282	 #,
 # array.hpp:181:             mStrides[mDim - i - 1] = multiplier;
	movl	%r8d, 32(%rdx)	 # multiplier, MEM[(long int &)_62 + 32]
.L282:
 # random.hpp:64:     }
	movq	%rbp, %rax	 # <retval>,
 # data.hpp:27:             mAccessCount--;
	movq	$1, 8(%r10)	 #, MEM[(struct Control *)_348].mAccessCount
 # random.hpp:64:     }
	addq	$72, %rsp	 #,
	popq	%rbx	 #
	popq	%rsi	 #
	popq	%rdi	 #
	popq	%rbp	 #
	popq	%r12	 #
	popq	%r13	 #
	popq	%r14	 #
	popq	%r15	 #
	ret	
.L259:
 # data.hpp:70:         mRaw = new T[size];
	movl	$8, %ecx	 #,
 # array.hpp:36:         long flatLength = 1;
	movl	$1, %r15d	 #, flatLength
 # random.hpp:59:         auto data = Data<T>(Array<T>::calculateFlatLength(shape));
	movl	$1, %esi	 #, _1
.LEHB22:
 # data.hpp:70:         mRaw = new T[size];
	call	_Znay	 #
 # data.hpp:71:         mControl = new Control(mRaw, size);
	movl	$32, %ecx	 #,
 # data.hpp:70:         mRaw = new T[size];
	movq	%rax, %r14	 # tmp349, _349
 # data.hpp:70:         mRaw = new T[size];
	movq	%rax, 48(%rsp)	 # _349, data.mRaw
 # data.hpp:71:         mControl = new Control(mRaw, size);
	call	_Znwy	 #
.LEHE22:
 # data.hpp:39:             mRaw = raw;
	movq	%r14, (%rax)	 # _349, MEM[(struct Control *)_358].mRaw
 # data.hpp:71:         mControl = new Control(mRaw, size);
	movq	%rax, %r10	 # tmp350, _347
 # data.hpp:40:             mSize = size;
	movq	$1, 16(%rax)	 #, MEM[(struct Control *)_358].mSize
 # data.hpp:41:             mAccessCount = 1;
	movq	$1, 8(%rax)	 #, MEM[(struct Control *)_358].mAccessCount
 # data.hpp:42:             mRealeased = false;
	movb	$0, 24(%rax)	 #, MEM[(struct Control *)_358].mRealeased
 # data.hpp:71:         mControl = new Control(mRaw, size);
	movq	%rax, 56(%rsp)	 # _347, data.mControl
	jmp	.L261	 #
.L358:
 # random.hpp:60:         for (size_t i = 0; i < data.size(); i++)
	xorl	%r15d, %r15d	 # flatLength
	jmp	.L280	 #
.L359:
 # data.hpp:152:         assertm(i >= 0 && i < size(), "Index out of bounds!");
	movl	$152, %r8d	 #,
	leaq	.LC0(%rip), %rdx	 #, tmp293
	leaq	.LC5(%rip), %rcx	 #, tmp294
.LEHB23:
	call	*__imp__assert(%rip)	 #
.LEHE23:
.L304:
.L357:
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	movq	%rax, %rbx	 # tmp359, tmp330
	jmp	.L294	 #
.L289:
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	movl	$16, %ecx	 #,
	call	__cxa_allocate_exception	 #
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	leaq	.LC4(%rip), %rdx	 #, tmp309
	movq	%rax, %rcx	 # _158,
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	movq	%rax, %rsi	 # tmp358, _158
.LEHB24:
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	call	_ZNSt12out_of_rangeC1EPKc	 #
.LEHE24:
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	leaq	_ZNSt12out_of_rangeD1Ev(%rip), %r8	 #,
	leaq	_ZTISt12out_of_range(%rip), %rdx	 #, tmp310
	movq	%rsi, %rcx	 # _158,
.LEHB25:
	call	__cxa_throw	 #
.LEHE25:
.L305:
	jmp	.L357	 #
.L294:
 # stack_buffer.hpp:126:             throw std::out_of_range("The index is out of range.");
	movq	%rsi, %rcx	 # _158,
	call	__cxa_free_exception	 #
.L284:
 # data.hpp:167:         if (mControl != nullptr)
	cmpq	$0, 8(%rbp)	 #, MEM[(struct Data *)_17(D)].mControl
	je	.L299	 #,
 # data.hpp:168:             release();
	movq	%rbp, %rcx	 # <retval>,
	call	_ZN4DataIdE7releaseEv	 #
.L299:
	leaq	48(%rsp), %rcx	 #, tmp337
	call	_ZN4DataIdE7releaseEv	 #
	movq	%rbx, %rcx	 # tmp334,
.LEHB26:
	call	_Unwind_Resume	 #
.LEHE26:
.L302:
 # array.hpp:86:     }
	movq	%rax, %rbx	 # tmp360, tmp299
	jmp	.L284	 #
.L360:
 # stack_buffer.hpp:29:             throw std::invalid_argument("Size cannot be bigger than buffer size.");
	movl	$16, %ecx	 #,
	call	__cxa_allocate_exception	 #
 # stack_buffer.hpp:29:             throw std::invalid_argument("Size cannot be bigger than buffer size.");
	leaq	.LC3(%rip), %rdx	 #, tmp297
	movq	%rax, %rcx	 # _134,
 # stack_buffer.hpp:29:             throw std::invalid_argument("Size cannot be bigger than buffer size.");
	movq	%rax, %rsi	 # tmp354, _134
.LEHB27:
 # stack_buffer.hpp:29:             throw std::invalid_argument("Size cannot be bigger than buffer size.");
	call	_ZNSt16invalid_argumentC1EPKc	 #
.LEHE27:
 # stack_buffer.hpp:29:             throw std::invalid_argument("Size cannot be bigger than buffer size.");
	leaq	_ZNSt16invalid_argumentD1Ev(%rip), %r8	 #,
	leaq	_ZTISt16invalid_argument(%rip), %rdx	 #, tmp298
	movq	%rsi, %rcx	 # _134,
.LEHB28:
	call	__cxa_throw	 #
.LEHE28:
.L262:
.LEHB29:
 # data.hpp:70:         mRaw = new T[size];
	call	__cxa_throw_bad_array_new_length	 #
.LEHE29:
.L301:
 # data.hpp:168:             release();
	movq	%rax, %rbx	 # tmp361, tmp334
	jmp	.L299	 #
.L303:
	jmp	.L357	 #
	.seh_handler	__gxx_personality_seh0, @unwind, @except
	.seh_handlerdata
.LLSDA6085:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE6085-.LLSDACSB6085
.LLSDACSB6085:
	.uleb128 .LEHB19-.LFB6085
	.uleb128 .LEHE19-.LEHB19
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB20-.LFB6085
	.uleb128 .LEHE20-.LEHB20
	.uleb128 .L304-.LFB6085
	.uleb128 0
	.uleb128 .LEHB21-.LFB6085
	.uleb128 .LEHE21-.LEHB21
	.uleb128 .L302-.LFB6085
	.uleb128 0
	.uleb128 .LEHB22-.LFB6085
	.uleb128 .LEHE22-.LEHB22
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB23-.LFB6085
	.uleb128 .LEHE23-.LEHB23
	.uleb128 .L301-.LFB6085
	.uleb128 0
	.uleb128 .LEHB24-.LFB6085
	.uleb128 .LEHE24-.LEHB24
	.uleb128 .L305-.LFB6085
	.uleb128 0
	.uleb128 .LEHB25-.LFB6085
	.uleb128 .LEHE25-.LEHB25
	.uleb128 .L302-.LFB6085
	.uleb128 0
	.uleb128 .LEHB26-.LFB6085
	.uleb128 .LEHE26-.LEHB26
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB27-.LFB6085
	.uleb128 .LEHE27-.LEHB27
	.uleb128 .L303-.LFB6085
	.uleb128 0
	.uleb128 .LEHB28-.LFB6085
	.uleb128 .LEHE28-.LEHB28
	.uleb128 .L302-.LFB6085
	.uleb128 0
	.uleb128 .LEHB29-.LFB6085
	.uleb128 .LEHE29-.LEHB29
	.uleb128 0
	.uleb128 0
.LLSDACSE6085:
	.section	.text$_ZN20RandomArrayGenerator14uniformIntegerIdEE5ArrayIT_ERK11StackBufferIlLy8EEll,"x"
	.linkonce discard
	.seh_endproc
	.section .rdata,"dr"
	.align 8
.LC8:
	.ascii "Output too long. Flat length is \0"
.LC9:
	.ascii "[\0"
.LC10:
	.ascii "\11\0"
.LC11:
	.ascii "]\0"
.LC12:
	.ascii ",\0"
	.section	.text$_ZNK5ArrayIdE9to_stringB5cxx11Ev,"x"
	.linkonce discard
	.align 2
	.p2align 4
	.globl	_ZNK5ArrayIdE9to_stringB5cxx11Ev
	.def	_ZNK5ArrayIdE9to_stringB5cxx11Ev;	.scl	2;	.type	32;	.endef
	.seh_proc	_ZNK5ArrayIdE9to_stringB5cxx11Ev
_ZNK5ArrayIdE9to_stringB5cxx11Ev:
.LFB6327:
	pushq	%r15	 #
	.seh_pushreg	%r15
	pushq	%r14	 #
	.seh_pushreg	%r14
	pushq	%r13	 #
	.seh_pushreg	%r13
	pushq	%r12	 #
	.seh_pushreg	%r12
	pushq	%rbp	 #
	.seh_pushreg	%rbp
	pushq	%rdi	 #
	.seh_pushreg	%rdi
	pushq	%rsi	 #
	.seh_pushreg	%rsi
	pushq	%rbx	 #
	.seh_pushreg	%rbx
	subq	$536, %rsp	 #,
	.seh_stackalloc	536
	movaps	%xmm6, 512(%rsp)	 #,
	.seh_savexmm	%xmm6, 512
	.seh_endprologue
 # array.hpp:50:     long getFlatLength() const { return mFlatLength; }
	movl	16(%rdx), %eax	 # this_38(D)->mFlatLength, _40
 # array.hpp:973:     std::string to_string() const
	movq	%rcx, %r15	 # tmp452, <retval>
	movq	%rdx, %rsi	 # tmp453, this
 # array.hpp:975:         if (getFlatLength() > 100)
	cmpl	$100, %eax	 #, _40
	jle	.L363	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:4194:     const unsigned long __uval = __neg ? (unsigned long)~__val + 1ul : __val;
	movl	%eax, %esi	 # _40, __val
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:69: 	  if (__value < __b3) return __n + 2;
	cmpl	$999, %eax	 #, __val
	jbe	.L420	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:70: 	  if (__value < __b4) return __n + 3;
	cmpl	$9999, %eax	 #, __val
	jbe	.L453	 #,
	movl	$5, %ebp	 #, __n
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:67: 	  if (__value < (unsigned)__base) return __n;
	cmpl	$99999, %eax	 #, __val
	jbe	.L370	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:68: 	  if (__value < __b2) return __n + 1;
	cmpl	$999999, %esi	 #, __val
	jbe	.L454	 #,
	movl	$6, %ebp	 #, __pos
	movl	$7, %edx	 #, _375
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:69: 	  if (__value < __b3) return __n + 2;
	cmpl	$9999999, %esi	 #, __val
	jbe	.L368	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:70: 	  if (__value < __b4) return __n + 3;
	cmpl	$99999999, %esi	 #, __val
	jbe	.L422	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:67: 	  if (__value < (unsigned)__base) return __n;
	cmpl	$999999999, %esi	 #, __val
	jbe	.L423	 #,
	movl	$5, %eax	 #, __n
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:72: 	  __n += 4;
	movl	$9, %ebp	 #, __pos
.L371:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:68: 	  if (__value < __b2) return __n + 1;
	leal	5(%rax), %edx	 #,
.L368:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:200: 	: allocator_type(__a), _M_p(__dat) { }
	leaq	96(%rsp), %rdi	 #, tmp444
	leaq	112(%rsp), %rbx	 #, tmp445
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:667:       { _M_construct(__n, __c); }
	movl	$45, %r8d	 #,
	movq	%rdi, %rcx	 # tmp444,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:200: 	: allocator_type(__a), _M_p(__dat) { }
	movq	%rbx, 96(%rsp)	 # tmp445, MEM[(struct _Alloc_hider *)_394]._M_p
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:667:       { _M_construct(__n, __c); }
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructEyc	 #
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:234:       { return _M_dataplus._M_p; }
	movq	96(%rsp), %rcx	 # MEM[(struct basic_string *)_394]._M_dataplus._M_p, _119
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:87:       constexpr char __digits[201] =
	movabsq	$3688503277381496880, %rax	 #, tmp487
	movabsq	$3976738051646829616, %rdx	 #,
	movq	%rax, 128(%rsp)	 # tmp487, MEM[(char[201] *)_402]
	movabsq	$3544667369688283184, %rax	 #, tmp488
	movq	%rdx, 136(%rsp)	 #, MEM[(char[201] *)_402]
	movabsq	$3832902143785906737, %rdx	 #,
	movq	%rax, 144(%rsp)	 # tmp488, MEM[(char[201] *)_402]
	movabsq	$4121136918051239473, %rax	 #, tmp489
	movq	%rdx, 152(%rsp)	 #, MEM[(char[201] *)_402]
	movabsq	$3689066235924983858, %rdx	 #,
	movq	%rax, 160(%rsp)	 # tmp489, MEM[(char[201] *)_402]
	movabsq	$3977301010190316594, %rax	 #, tmp490
	movq	%rdx, 168(%rsp)	 #, MEM[(char[201] *)_402]
	movabsq	$3545230328231770162, %rdx	 #,
	movq	%rax, 176(%rsp)	 # tmp490, MEM[(char[201] *)_402]
	movabsq	$3833465102329393715, %rax	 #, tmp491
	movq	%rdx, 184(%rsp)	 #, MEM[(char[201] *)_402]
	movabsq	$4121699876594726451, %rdx	 #,
	movq	%rax, 192(%rsp)	 # tmp491, MEM[(char[201] *)_402]
	movabsq	$3689629194468470836, %rax	 #, tmp492
	movq	%rdx, 200(%rsp)	 #, MEM[(char[201] *)_402]
	movabsq	$3977863968733803572, %rdx	 #,
	movq	%rax, 208(%rsp)	 # tmp492, MEM[(char[201] *)_402]
	movabsq	$3545793286775257140, %rax	 #, tmp493
	movq	%rdx, 216(%rsp)	 #, MEM[(char[201] *)_402]
	movabsq	$3834028060872880693, %rdx	 #,
	movq	%rax, 224(%rsp)	 # tmp493, MEM[(char[201] *)_402]
	movabsq	$4122262835138213429, %rax	 #, tmp494
	movq	%rdx, 232(%rsp)	 #, MEM[(char[201] *)_402]
	movabsq	$3690192153011957814, %rdx	 #,
	movq	%rax, 240(%rsp)	 # tmp494, MEM[(char[201] *)_402]
	movabsq	$3978426927277290550, %rax	 #, tmp495
	movq	%rdx, 248(%rsp)	 #, MEM[(char[201] *)_402]
	movabsq	$3546356245318744118, %rdx	 #,
	movq	%rax, 256(%rsp)	 # tmp495, MEM[(char[201] *)_402]
	movabsq	$3834591019416367671, %rax	 #, tmp496
	movq	%rdx, 264(%rsp)	 #, MEM[(char[201] *)_402]
	movabsq	$4122825793681700407, %rdx	 #,
	movq	%rax, 272(%rsp)	 # tmp496, MEM[(char[201] *)_402]
	movabsq	$3690755111555444792, %rax	 #, tmp497
	movq	%rdx, 280(%rsp)	 #, MEM[(char[201] *)_402]
	movabsq	$3978989885820777528, %rdx	 #,
	movq	%rax, 288(%rsp)	 # tmp497, MEM[(char[201] *)_402]
	movabsq	$3546919203862231096, %rax	 #, tmp498
	movq	%rdx, 296(%rsp)	 #, MEM[(char[201] *)_402]
	movabsq	$3835153977959854649, %rdx	 #,
	movq	%rdx, 312(%rsp)	 #, MEM[(char[201] *)_402]
	movabsq	$16106987313379638, %rdx	 #,
	movq	%rax, 304(%rsp)	 # tmp498, MEM[(char[201] *)_402]
	movabsq	$4122263930388298034, %rax	 #, tmp499
	movq	%rax, 313(%rsp)	 # tmp499, MEM[(char[201] *)_402]
	movq	%rdx, 321(%rsp)	 #, MEM[(char[201] *)_402]
	.p2align 4,,10
	.p2align 3
.L373:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:96: 	  auto const __num = (__val % 100) * 2;
	movl	%esi, %edx	 # __val, __val
	movl	%esi, %eax	 # __val, tmp208
	imulq	$1374389535, %rdx, %rdx	 #, __val, tmp205
	shrq	$37, %rdx	 #, tmp203
	imull	$100, %edx, %r8d	 #, tmp203, tmp207
	subl	%r8d, %eax	 # tmp207, tmp208
	movl	%esi, %r8d	 # __val, __val
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:97: 	  __val /= 100;
	movl	%edx, %esi	 # tmp203, __val
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:98: 	  __first[__pos] = __digits[__num + 1];
	movl	%ebp, %edx	 # __pos, __pos
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:96: 	  auto const __num = (__val % 100) * 2;
	addl	%eax, %eax	 #
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:98: 	  __first[__pos] = __digits[__num + 1];
	leal	1(%rax), %r9d	 #, tmp229
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:98: 	  __first[__pos] = __digits[__num + 1];
	movzbl	128(%rsp,%r9), %r9d	 # MEM[(char[201] *)_402][_127], MEM[(char[201] *)_402][_127]
	movb	%r9b, (%rcx,%rdx)	 # MEM[(char[201] *)_402][_127], *_130
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:99: 	  __first[__pos - 1] = __digits[__num];
	movzbl	128(%rsp,%rax), %eax	 # MEM[(char[201] *)_402][__num_125], MEM[(char[201] *)_402][__num_125]
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:99: 	  __first[__pos - 1] = __digits[__num];
	leal	-1(%rbp), %edx	 #, tmp232
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:100: 	  __pos -= 2;
	subl	$2, %ebp	 #, __pos
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:99: 	  __first[__pos - 1] = __digits[__num];
	movb	%al, (%rcx,%rdx)	 # MEM[(char[201] *)_402][__num_125], *_134
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:94:       while (__val >= 100)
	cmpl	$9999, %r8d	 #, __val
	ja	.L373	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:109: 	__first[0] = '0' + __val;
	leal	48(%rsi), %eax	 #, cstore_121
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:102:       if (__val >= 10)
	cmpl	$999, %r8d	 #, __val
	jbe	.L375	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:104: 	  auto const __num = __val * 2;
	addl	%esi, %esi	 #
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:105: 	  __first[1] = __digits[__num + 1];
	leal	1(%rsi), %eax	 #, tmp236
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:105: 	  __first[1] = __digits[__num + 1];
	movzbl	128(%rsp,%rax), %eax	 # MEM[(char[201] *)_402][_139], MEM[(char[201] *)_402][_139]
	movb	%al, 1(%rcx)	 # MEM[(char[201] *)_402][_139], MEM[(char *)_119 + 1B]
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:106: 	  __first[0] = __digits[__num];
	movzbl	128(%rsp,%rsi), %eax	 # MEM[(char[201] *)_402][__num_138], cstore_121
.L375:
	movb	%al, (%rcx)	 # cstore_121, *_119
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:2207: 	return _M_replace(_M_check(__pos, "basic_string::replace"),
	leaq	.LC8(%rip), %r9	 #,
	xorl	%r8d, %r8d	 #
	xorl	%edx, %edx	 #
	movq	$32, 32(%rsp)	 #,
	movq	%rdi, %rcx	 # tmp444,
.LEHB30:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_replaceEyyPKcy	 #
.LEHE30:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:241: 	return std::pointer_traits<pointer>::pointer_to(*_M_local_buf);
	leaq	16(%r15), %r8	 #, _155
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:252: 	return std::pointer_traits<const_pointer>::pointer_to(*_M_local_buf);
	leaq	16(%rax), %r9	 #, _157
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:277: 	    if (_M_string_length > _S_local_capacity)
	movq	8(%rax), %rcx	 # MEM[(const struct basic_string *)_148]._M_string_length, MEM[(const struct basic_string *)_148]._M_string_length
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:204: 	: allocator_type(std::move(__a)), _M_p(__dat) { }
	movq	%r8, (%r15)	 # _155, MEM[(struct _Alloc_hider *)_41(D)]._M_p
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:234:       { return _M_dataplus._M_p; }
	movq	(%rax), %rdx	 # MEM[(const struct basic_string *)_148]._M_dataplus._M_p, _156
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:275: 	if (_M_data() == _M_local_data())
	cmpq	%r9, %rdx	 # _157, _156
	je	.L455	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:224:       { _M_dataplus._M_p = __p; }
	movq	%rdx, (%r15)	 # _156, MEM[(struct basic_string *)_41(D)]._M_dataplus._M_p
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:261:       { _M_allocated_capacity = __capacity; }
	movq	16(%rax), %rdx	 # *_148.D.29453._M_allocated_capacity, *_148.D.29453._M_allocated_capacity
	movq	%rdx, 16(%r15)	 # *_148.D.29453._M_allocated_capacity, MEM[(struct basic_string *)_41(D)].D.29453._M_allocated_capacity
.L383:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:229:       { _M_string_length = __length; }
	movq	%rcx, 8(%r15)	 # MEM[(const struct basic_string *)_148]._M_string_length, MEM[(struct basic_string *)_41(D)]._M_string_length
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:224:       { _M_dataplus._M_p = __p; }
	movq	%r9, (%rax)	 # _157, *_148._M_dataplus._M_p
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/char_traits.h:368: 	__c1 = __c2;
	movb	$0, 16(%rax)	 #, MEM[(char_type &)_148 + 16]
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:234:       { return _M_dataplus._M_p; }
	movq	96(%rsp), %rcx	 # MEM[(struct basic_string *)_394]._M_dataplus._M_p, _149
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:229:       { _M_string_length = __length; }
	movq	$0, 8(%rax)	 #, *_148._M_string_length
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:275: 	if (_M_data() == _M_local_data())
	cmpq	%rbx, %rcx	 # tmp445, _149
	je	.L362	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:300:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	112(%rsp), %rax	 # MEM[(struct basic_string *)_394].D.29453._M_allocated_capacity, tmp502
	leaq	1(%rax), %rdx	 #, tmp280
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/new_allocator.h:168: 	_GLIBCXX_OPERATOR_DELETE(_GLIBCXX_SIZED_DEALLOC(__p, __n));
	call	_ZdlPvy	 #
	nop	
.L362:
 # array.hpp:1011:     }
	movaps	512(%rsp), %xmm6	 #,
	movq	%r15, %rax	 # <retval>,
	addq	$536, %rsp	 #,
	popq	%rbx	 #
	popq	%rsi	 #
	popq	%rdi	 #
	popq	%rbp	 #
	popq	%r12	 #
	popq	%r13	 #
	popq	%r14	 #
	popq	%r15	 #
	ret	
	.p2align 4,,10
	.p2align 3
.L363:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_ios.h:462: 	_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
	leaq	240(%rsp), %rdi	 #, tmp443
	leaq	128(%rsp), %rbp	 #, tmp446
	movq	%rdi, %rcx	 # tmp443,
	movq	%rdi, 80(%rsp)	 # tmp443, %sfp
	call	_ZNSt8ios_baseC2Ev	 #
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_ios.h:462: 	_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
	movq	.refptr._ZTVSt9basic_iosIcSt11char_traitsIcEE(%rip), %rax	 #, tmp504
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_ios.h:461:       : ios_base(), _M_tie(0), _M_fill(char_type()), _M_fill_init(false), 
	xorl	%edx, %edx	 #
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_ios.h:462: 	_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
	pxor	%xmm0, %xmm0	 # tmp286
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_ios.h:461:       : ios_base(), _M_tie(0), _M_fill(char_type()), _M_fill_init(false), 
	movw	%dx, 464(%rsp)	 #, MEM <unsigned short> [(void *)_402]
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_ios.h:462: 	_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
	addq	$16, %rax	 #, tmp285
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_ios.h:462: 	_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
	movups	%xmm0, 472(%rsp)	 # tmp286, MEM <vector(2) long long unsigned int> [(void *)_402]
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_ios.h:462: 	_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
	movq	%rax, 240(%rsp)	 # tmp285, MEM[(struct basic_ios *)_402].D.48244._vptr.ios_base
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:432:       { this->init(0); }
	movq	.refptr._ZTTNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE(%rip), %rax	 #, tmp288
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_ios.h:462: 	_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
	movups	%xmm0, 488(%rsp)	 # tmp286, MEM <vector(2) long long unsigned int> [(void *)_402]
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_ios.h:461:       : ios_base(), _M_tie(0), _M_fill(char_type()), _M_fill_init(false), 
	movq	$0, 456(%rsp)	 #, MEM[(struct basic_ios *)_402]._M_tie
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:432:       { this->init(0); }
	movq	8(%rax), %rdx	 # MEM[(const void * *)&_ZTTNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE + 8B], _165
	movq	16(%rax), %rax	 # MEM[(const void * *)&_ZTTNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE + 16B], _169
	movq	-24(%rdx), %rcx	 # MEM[(long long int *)_165 + -24B], _168
	movq	%rdx, 128(%rsp)	 # _165, MEM[(struct basic_ostream *)_402]._vptr.basic_ostream
	movq	%rdx, 56(%rsp)	 # _165, %sfp
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:432:       { this->init(0); }
	xorl	%edx, %edx	 #
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:432:       { this->init(0); }
	addq	%rbp, %rcx	 # tmp446, _168
	movq	%rax, 64(%rsp)	 # _169, %sfp
	movq	%rax, (%rcx)	 # _169, MEM[(struct basic_ios *)_168].D.48244._vptr.ios_base
.LEHB31:
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:432:       { this->init(0); }
	call	_ZNSt9basic_iosIcSt11char_traitsIcEE4initEPSt15basic_streambufIcS1_E	 #
.LEHE31:
	movq	.refptr._ZTVNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE(%rip), %rax	 #, tmp512
	addq	$24, %rax	 #, _189
	movq	%rax, %xmm6	 # _189, _189
 # C:/Program Files/Perl/c/include/c++/13.1.0/sstream:805:       : __ostream_type(), _M_stringbuf(ios_base::out)
	movq	.refptr._ZTVNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE(%rip), %rax	 #, tmp513
	movdqa	%xmm6, %xmm0	 # _189, tmp295
	addq	$64, %rax	 #, tmp294
	movq	%rax, 240(%rsp)	 # tmp294, MEM[(struct basic_ios *)_402].D.48244._vptr.ios_base
	movq	.refptr._ZTVSt15basic_streambufIcSt11char_traitsIcEE(%rip), %rax	 #, tmp514
	addq	$16, %rax	 #, tmp296
	movq	%rax, %xmm2	 # tmp296, tmp296
 # C:/Program Files/Perl/c/include/c++/13.1.0/streambuf:473:       _M_buf_locale(locale())
	leaq	192(%rsp), %rax	 #, tmp438
 # C:/Program Files/Perl/c/include/c++/13.1.0/sstream:805:       : __ostream_type(), _M_stringbuf(ios_base::out)
	punpcklqdq	%xmm2, %xmm0	 # tmp296, tmp295
 # C:/Program Files/Perl/c/include/c++/13.1.0/streambuf:473:       _M_buf_locale(locale())
	movq	%rax, %rcx	 # tmp438,
	movq	%rax, 72(%rsp)	 # tmp438, %sfp
 # C:/Program Files/Perl/c/include/c++/13.1.0/sstream:805:       : __ostream_type(), _M_stringbuf(ios_base::out)
	movaps	%xmm0, 128(%rsp)	 # tmp295, MEM <vector(2) long long unsigned int> [(int (*) () * *)_402]
 # C:/Program Files/Perl/c/include/c++/13.1.0/streambuf:471:       : _M_in_beg(0), _M_in_cur(0), _M_in_end(0),
	pxor	%xmm0, %xmm0	 # tmp298
	movaps	%xmm0, 144(%rsp)	 # tmp298, MEM <vector(2) long long unsigned int> [(char_type * *)_402]
	movaps	%xmm0, 160(%rsp)	 # tmp298, MEM <vector(2) long long unsigned int> [(char_type * *)_402]
	movaps	%xmm0, 176(%rsp)	 # tmp298, MEM <vector(2) long long unsigned int> [(char_type * *)_402]
 # C:/Program Files/Perl/c/include/c++/13.1.0/streambuf:473:       _M_buf_locale(locale())
	call	_ZNSt6localeC1Ev	 #
 # C:/Program Files/Perl/c/include/c++/13.1.0/sstream:134:       : __streambuf_type(), _M_mode(__mode), _M_string()
	movq	.refptr._ZTVNSt7__cxx1115basic_stringbufIcSt11char_traitsIcESaIcEEE(%rip), %rax	 #, tmp518
 # C:/Program Files/Perl/c/include/c++/13.1.0/sstream:806:       { this->init(&_M_stringbuf); }
	movq	%rdi, %rcx	 # tmp443,
	leaq	136(%rsp), %rdx	 #, tmp308
 # C:/Program Files/Perl/c/include/c++/13.1.0/sstream:134:       : __streambuf_type(), _M_mode(__mode), _M_string()
	movl	$16, 200(%rsp)	 #, MEM[(struct basic_stringbuf *)_402]._M_mode
 # C:/Program Files/Perl/c/include/c++/13.1.0/sstream:134:       : __streambuf_type(), _M_mode(__mode), _M_string()
	addq	$16, %rax	 #, tmp304
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/char_traits.h:368: 	__c1 = __c2;
	movb	$0, 224(%rsp)	 #, MEM[(char_type &)_402]
 # C:/Program Files/Perl/c/include/c++/13.1.0/sstream:134:       : __streambuf_type(), _M_mode(__mode), _M_string()
	movq	%rax, 136(%rsp)	 # tmp304, MEM[(struct basic_stringbuf *)_402].D.81137._vptr.basic_streambuf
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:204: 	: allocator_type(std::move(__a)), _M_p(__dat) { }
	leaq	224(%rsp), %rax	 #, tmp440
	movq	%rax, 88(%rsp)	 # tmp440, %sfp
	movq	%rax, 208(%rsp)	 # tmp440, MEM[(struct _Alloc_hider *)_402]._M_p
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:229:       { _M_string_length = __length; }
	movq	$0, 216(%rsp)	 #, MEM[(struct basic_string *)_402]._M_string_length
.LEHB32:
 # C:/Program Files/Perl/c/include/c++/13.1.0/sstream:806:       { this->init(&_M_stringbuf); }
	call	_ZNSt9basic_iosIcSt11char_traitsIcEE4initEPSt15basic_streambufIcS1_E	 #
.LEHE32:
 # array.hpp:981:         for (size_t i = 0; i < getFlatLength(); i++)
	movl	16(%rsi), %eax	 # this_38(D)->mFlatLength,
 # array.hpp:981:         for (size_t i = 0; i < getFlatLength(); i++)
	xorl	%r14d, %r14d	 # i
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:667: 	__ostream_insert(__out, __s,
	leaq	.LC9(%rip), %r13	 #, tmp449
 # array.hpp:981:         for (size_t i = 0; i < getFlatLength(); i++)
	testl	%eax, %eax	 #
	je	.L412	 #,
	.p2align 4,,10
	.p2align 3
.L386:
 # array.hpp:984:             for (long j = mDim - 1; j >= 0; j--)
	movl	108(%rsi), %ebx	 # this_38(D)->mDim, j
	subl	$1, %ebx	 #, j
	js	.L391	 #,
 # stack_buffer.hpp:133:         if (i >= mSize)
	movq	56(%rsi), %rcx	 # MEM[(const struct StackBuffer *)this_38(D) + 24B].mSize, prephitmp_117
	movslq	%ebx, %rbx	 # j, ivtmp.375
 # array.hpp:983:             m = i;
	movq	%r14, %rdi	 # i, m
	jmp	.L397	 #
	.p2align 4,,10
	.p2align 3
.L393:
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:667: 	__ostream_insert(__out, __s,
	movl	$1, %r8d	 #,
	movq	%r13, %rdx	 # tmp449,
	movq	%rbp, %rcx	 # tmp446,
.LEHB33:
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x	 #
 # stack_buffer.hpp:133:         if (i >= mSize)
	movq	56(%rsi), %rcx	 # MEM[(const struct StackBuffer *)this_38(D) + 24B].mSize, prephitmp_117
 # stack_buffer.hpp:133:         if (i >= mSize)
	cmpq	%rcx, %rbx	 # prephitmp_117, ivtmp.375
	jnb	.L456	 #,
 # array.hpp:990:                 m /= mShape[j];
	movslq	24(%rsi,%rbx,4), %r8	 # MEM[(const long int &)this_38(D) + 24 + ivtmp.375_238 * 4], MEM[(const long int &)this_38(D) + 24 + ivtmp.375_238 * 4]
	movq	%rdi, %rax	 # m, m
	xorl	%edx, %edx	 # tmp352
 # array.hpp:984:             for (long j = mDim - 1; j >= 0; j--)
	subq	$1, %rbx	 #, ivtmp.375
 # array.hpp:990:                 m /= mShape[j];
	divq	%r8	 # MEM[(const long int &)this_38(D) + 24 + ivtmp.375_238 * 4]
	movq	%rax, %rdi	 # m, m
 # array.hpp:984:             for (long j = mDim - 1; j >= 0; j--)
	testl	%ebx, %ebx	 # ivtmp.375
	js	.L391	 #,
.L397:
 # stack_buffer.hpp:133:         if (i >= mSize)
	cmpq	%rcx, %rbx	 # prephitmp_117, ivtmp.375
	jnb	.L457	 #,
 # array.hpp:986:                 if (m % mShape[j] == 0)
	movslq	24(%rsi,%rbx,4), %rcx	 # MEM[(const long int &)this_38(D) + 24 + ivtmp.375_238 * 4], MEM[(const long int &)this_38(D) + 24 + ivtmp.375_238 * 4]
	movq	%rdi, %rax	 # m, tmp339
	xorl	%edx, %edx	 # tmp338
	divq	%rcx	 # MEM[(const long int &)this_38(D) + 24 + ivtmp.375_238 * 4]
 # array.hpp:986:                 if (m % mShape[j] == 0)
	testq	%rdx, %rdx	 # tmp338
	je	.L393	 #,
.L391:
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:667: 	__ostream_insert(__out, __s,
	movl	$1, %r8d	 #,
	leaq	.LC10(%rip), %rdx	 #,
	movq	%rbp, %rcx	 # tmp446,
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x	 #
 # data.hpp:152:         assertm(i >= 0 && i < size(), "Index out of bounds!");
	movl	$2147483648, %eax	 #, tmp357
	movq	(%rsi), %rdx	 # MEM[(double * *)this_38(D)], _94
	movq	8(%rsi), %rcx	 # MEM[(struct Control * *)this_38(D) + 8B], _95
	cmpq	%rax, %r14	 # tmp357, i
	je	.L399	 #,
 # data.hpp:152:         assertm(i >= 0 && i < size(), "Index out of bounds!");
	cmpq	16(%rcx), %r14	 # _95->mSize, i
	jnb	.L399	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:223:       { return _M_insert(__f); }
	movsd	(%rdx,%r14,8), %xmm1	 # *_191,
	movq	%rbp, %rcx	 # tmp446,
	call	_ZNSo9_M_insertIdEERSoT_	 #
 # array.hpp:997:             for (long j = mDim - 1; j >= 0; j--)
	movl	108(%rsi), %ebx	 # this_38(D)->mDim, j
	subl	$1, %ebx	 #, j
	js	.L408	 #,
 # stack_buffer.hpp:133:         if (i >= mSize)
	movq	56(%rsi), %rcx	 # MEM[(const struct StackBuffer *)this_38(D) + 24B].mSize, prephitmp_97
	movslq	%ebx, %rbx	 # j, ivtmp.364
 # array.hpp:995:             m = i;
	movq	%r14, %rdi	 # i, m
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:667: 	__ostream_insert(__out, __s,
	leaq	.LC11(%rip), %r12	 #, tmp447
	jmp	.L410	 #
	.p2align 4,,10
	.p2align 3
.L403:
	movl	$1, %r8d	 #,
	movq	%r12, %rdx	 # tmp447,
	movq	%rbp, %rcx	 # tmp446,
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x	 #
 # stack_buffer.hpp:133:         if (i >= mSize)
	movq	56(%rsi), %rcx	 # MEM[(const struct StackBuffer *)this_38(D) + 24B].mSize, prephitmp_97
 # stack_buffer.hpp:133:         if (i >= mSize)
	cmpq	%rcx, %rbx	 # prephitmp_97, ivtmp.364
	jnb	.L458	 #,
 # array.hpp:1006:                 m /= mShape[j];
	movslq	24(%rsi,%rbx,4), %r8	 # MEM[(const long int &)this_38(D) + 24 + ivtmp.364_223 * 4], MEM[(const long int &)this_38(D) + 24 + ivtmp.364_223 * 4]
	movq	%rdi, %rax	 # m, m
	xorl	%edx, %edx	 # tmp397
 # array.hpp:997:             for (long j = mDim - 1; j >= 0; j--)
	subq	$1, %rbx	 #, ivtmp.364
 # array.hpp:1006:                 m /= mShape[j];
	divq	%r8	 # MEM[(const long int &)this_38(D) + 24 + ivtmp.364_223 * 4]
	movq	%rax, %rdi	 # m, m
 # array.hpp:997:             for (long j = mDim - 1; j >= 0; j--)
	testl	%ebx, %ebx	 # ivtmp.364
	js	.L408	 #,
.L410:
 # array.hpp:999:                 if ((m + 1) % mShape[j] == 0)
	leaq	1(%rdi), %rax	 #, _12
 # stack_buffer.hpp:133:         if (i >= mSize)
	cmpq	%rcx, %rbx	 # prephitmp_97, ivtmp.364
	jnb	.L459	 #,
 # array.hpp:999:                 if ((m + 1) % mShape[j] == 0)
	movslq	24(%rsi,%rbx,4), %rcx	 # MEM[(const long int &)this_38(D) + 24 + ivtmp.364_223 * 4], MEM[(const long int &)this_38(D) + 24 + ivtmp.364_223 * 4]
	xorl	%edx, %edx	 # tmp370
	divq	%rcx	 # MEM[(const long int &)this_38(D) + 24 + ivtmp.364_223 * 4]
 # array.hpp:999:                 if ((m + 1) % mShape[j] == 0)
	testq	%rdx, %rdx	 # tmp370
	je	.L403	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:667: 	__ostream_insert(__out, __s,
	movl	$1, %r8d	 #,
	leaq	.LC12(%rip), %rdx	 #, tmp377
	movq	%rbp, %rcx	 # tmp446,
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x	 #
 # array.hpp:1003:                     oss << "," << std::string(mDim - 1 - j, '\n');
	notl	%ebx	 # tmp378
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:200: 	: allocator_type(__a), _M_p(__dat) { }
	leaq	96(%rsp), %rdi	 #, tmp444
 # array.hpp:1003:                     oss << "," << std::string(mDim - 1 - j, '\n');
	addl	108(%rsi), %ebx	 # this_38(D)->mDim, tmp380
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:667:       { _M_construct(__n, __c); }
	movl	$10, %r8d	 #,
 # array.hpp:1003:                     oss << "," << std::string(mDim - 1 - j, '\n');
	movslq	%ebx, %rdx	 # tmp380, _21
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:667:       { _M_construct(__n, __c); }
	movq	%rdi, %rcx	 # tmp444,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:200: 	: allocator_type(__a), _M_p(__dat) { }
	leaq	112(%rsp), %rbx	 #, tmp445
	movq	%rbx, 96(%rsp)	 # tmp445, MEM[(struct _Alloc_hider *)_394]._M_p
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:667:       { _M_construct(__n, __c); }
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructEyc	 #
.LEHE33:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:4036:       return __ostream_insert(__os, __str.data(), __str.size());
	movq	104(%rsp), %r8	 # MEM[(struct basic_string *)_394]._M_string_length,
	movq	96(%rsp), %rdx	 # MEM[(struct basic_string *)_394]._M_dataplus._M_p,
	movq	%rbp, %rcx	 # tmp446,
.LEHB34:
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x	 #
.LEHE34:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:234:       { return _M_dataplus._M_p; }
	movq	96(%rsp), %rcx	 # MEM[(struct basic_string *)_394]._M_dataplus._M_p, _197
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:275: 	if (_M_data() == _M_local_data())
	cmpq	%rbx, %rcx	 # tmp445, _197
	je	.L408	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:300:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	112(%rsp), %rax	 # MEM[(struct basic_string *)_394].D.29453._M_allocated_capacity, tmp531
	leaq	1(%rax), %rdx	 #, tmp389
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/new_allocator.h:168: 	_GLIBCXX_OPERATOR_DELETE(_GLIBCXX_SIZED_DEALLOC(__p, __n));
	call	_ZdlPvy	 #
.L408:
 # array.hpp:981:         for (size_t i = 0; i < getFlatLength(); i++)
	movslq	16(%rsi), %rax	 # this_38(D)->mFlatLength, this_38(D)->mFlatLength
 # array.hpp:981:         for (size_t i = 0; i < getFlatLength(); i++)
	addq	$1, %r14	 #, i
 # array.hpp:981:         for (size_t i = 0; i < getFlatLength(); i++)
	cmpq	%rax, %r14	 # this_38(D)->mFlatLength, i
	jb	.L386	 #,
.L412:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:241: 	return std::pointer_traits<pointer>::pointer_to(*_M_local_buf);
	leaq	16(%r15), %rax	 #, tmp311
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:229:       { _M_string_length = __length; }
	movq	$0, 8(%r15)	 #, MEM[(struct basic_string *)_41(D)]._M_string_length
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:241: 	return std::pointer_traits<pointer>::pointer_to(*_M_local_buf);
	movq	%rax, (%r15)	 # tmp311, MEM[(struct _Alloc_hider *)_41(D)]._M_p
 # C:/Program Files/Perl/c/include/c++/13.1.0/streambuf:539:       pptr() const { return _M_out_cur; }
	movq	176(%rsp), %rax	 # MEM[(const struct basic_streambuf *)_402]._M_out_cur, _209
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/char_traits.h:368: 	__c1 = __c2;
	movb	$0, 16(%r15)	 #, MEM[(char_type &)_41(D) + 16]
 # C:/Program Files/Perl/c/include/c++/13.1.0/sstream:442: 	if (char_type* __pptr = this->pptr())
	testq	%rax, %rax	 # _209
	je	.L460	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/streambuf:495:       egptr() const { return _M_in_end; }
	movq	160(%rsp), %rdx	 # MEM[(const struct basic_streambuf *)_402]._M_in_end, _210
 # C:/Program Files/Perl/c/include/c++/13.1.0/sstream:448: 	      return __egptr; // Underlying sequence is [pbase, egptr).
	movq	%rax, %rcx	 # _209, tmp470
 # C:/Program Files/Perl/c/include/c++/13.1.0/streambuf:536:       pbase() const { return _M_out_beg; }
	movq	168(%rsp), %r9	 # MEM[(const struct basic_streambuf *)_402]._M_out_beg, _214
 # C:/Program Files/Perl/c/include/c++/13.1.0/sstream:448: 	      return __egptr; // Underlying sequence is [pbase, egptr).
	cmpq	%rax, %rdx	 # _209, _210
	cmovnb	%rdx, %rax	 # _210,, tmp469
	testq	%rdx, %rdx	 # _210
	cmove	%rcx, %rax	 # tmp469,, tmp470, _209
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:2207: 	return _M_replace(_M_check(__pos, "basic_string::replace"),
	xorl	%r8d, %r8d	 #
	xorl	%edx, %edx	 #
	movq	%r15, %rcx	 # <retval>,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:2405: 			     __k1, __k2 - __k1);
	subq	%r9, %rax	 # _214, tmp406
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:2207: 	return _M_replace(_M_check(__pos, "basic_string::replace"),
	movq	%rax, 32(%rsp)	 # tmp406,
.LEHB35:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_replaceEyyPKcy	 #
.L414:
 # C:/Program Files/Perl/c/include/c++/13.1.0/sstream:851:       { }
	movq	.refptr._ZTVNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE(%rip), %rax	 #, tmp533
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:234:       { return _M_dataplus._M_p; }
	movq	208(%rsp), %rcx	 # MEM[(const struct basic_string *)_402]._M_dataplus._M_p, _232
 # C:/Program Files/Perl/c/include/c++/13.1.0/sstream:851:       { }
	addq	$64, %rax	 #, tmp410
	movq	%rax, 240(%rsp)	 # tmp410, MEM[(struct basic_ios *)_402].D.48244._vptr.ios_base
	movq	.refptr._ZTVNSt7__cxx1115basic_stringbufIcSt11char_traitsIcESaIcEEE(%rip), %rax	 #, tmp534
	addq	$16, %rax	 #, tmp412
	movq	%rax, %xmm3	 # tmp412, tmp412
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:275: 	if (_M_data() == _M_local_data())
	movq	88(%rsp), %rax	 # %sfp, tmp440
 # C:/Program Files/Perl/c/include/c++/13.1.0/sstream:851:       { }
	punpcklqdq	%xmm3, %xmm6	 # tmp412, tmp411
	movaps	%xmm6, 128(%rsp)	 # tmp411, MEM <vector(2) long long unsigned int> [(int (*) () * *)_402]
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:275: 	if (_M_data() == _M_local_data())
	cmpq	%rax, %rcx	 # tmp440, _232
	je	.L415	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:300:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	224(%rsp), %rax	 # MEM[(struct basic_string *)_402].D.29453._M_allocated_capacity, tmp537
	leaq	1(%rax), %rdx	 #, tmp418
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/new_allocator.h:168: 	_GLIBCXX_OPERATOR_DELETE(_GLIBCXX_SIZED_DEALLOC(__p, __n));
	call	_ZdlPvy	 #
.L415:
 # C:/Program Files/Perl/c/include/c++/13.1.0/streambuf:205:       { }
	movq	.refptr._ZTVSt15basic_streambufIcSt11char_traitsIcEE(%rip), %rax	 #, tmp538
	movq	72(%rsp), %rcx	 # %sfp,
	addq	$16, %rax	 #, tmp421
	movq	%rax, 136(%rsp)	 # tmp421, MEM[(struct basic_streambuf *)_402]._vptr.basic_streambuf
	call	_ZNSt6localeD1Ev	 #
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:95:       ~basic_ostream() { }
	movq	56(%rsp), %rax	 # %sfp, _165
	movq	64(%rsp), %rdi	 # %sfp, _169
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_ios.h:282:       ~basic_ios() { }
	movq	80(%rsp), %rcx	 # %sfp,
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:95:       ~basic_ostream() { }
	movq	%rax, 128(%rsp)	 # _165, MEM[(struct basic_ostream *)_402]._vptr.basic_ostream
	movq	-24(%rax), %rax	 # MEM[(long long int *)_165 + -24B], MEM[(long long int *)_165 + -24B]
	movq	%rdi, 128(%rsp,%rax)	 # _169, MEM[(struct basic_ios *)_229].D.48244._vptr.ios_base
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_ios.h:282:       ~basic_ios() { }
	movq	.refptr._ZTVSt9basic_iosIcSt11char_traitsIcEE(%rip), %rax	 #, tmp542
	addq	$16, %rax	 #, tmp426
	movq	%rax, 240(%rsp)	 # tmp426, MEM[(struct basic_ios *)_402].D.48244._vptr.ios_base
	call	_ZNSt8ios_baseD2Ev	 #
 # C:/Program Files/Perl/c/include/c++/13.1.0/sstream:851:       { }
	jmp	.L362	 #
.L460:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:1607: 	this->_M_assign(__str);
	leaq	208(%rsp), %rdx	 #, tmp408
	movq	%r15, %rcx	 # <retval>,
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_assignERKS4_	 #
.LEHE35:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:813: 	return this->assign(__str);
	jmp	.L414	 #
.L423:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:72: 	  __n += 4;
	movl	$9, %ebp	 #, __n
.L370:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:98: 	  __first[__pos] = __digits[__num + 1];
	movl	%ebp, %edx	 # __n, _375
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:99: 	  __first[__pos - 1] = __digits[__num];
	subl	$1, %ebp	 #, __pos
	jmp	.L368	 #
.L422:
	movl	$7, %ebp	 #, __pos
	movl	$8, %edx	 #, _375
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:70: 	  if (__value < __b4) return __n + 3;
	jmp	.L368	 #
.L455:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:683: 	    traits_type::copy(_M_local_buf, __str._M_local_buf,
	leaq	1(%rcx), %rdx	 #, tmp240
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/char_traits.h:445: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	cmpl	$8, %edx	 #, tmp240
	jnb	.L377	 #,
	testb	$4, %dl	 #, tmp240
	jne	.L461	 #,
	testl	%edx, %edx	 # tmp240
	je	.L383	 #,
	movzbl	16(%rax), %ecx	 #, tmp253
	movb	%cl, 16(%r15)	 # tmp253,
	testb	$2, %dl	 #, tmp240
	jne	.L445	 #,
.L451:
	movq	8(%rax), %rcx	 # MEM[(const struct basic_string *)_148]._M_string_length, MEM[(const struct basic_string *)_148]._M_string_length
	jmp	.L383	 #
.L377:
	movq	16(%rax), %rcx	 #, tmp262
	movq	%r9, %rsi	 # _157, _157
	movq	%rcx, 16(%r15)	 # tmp262,
	movl	%edx, %ecx	 # tmp240, tmp263
	movq	-8(%r9,%rcx), %r10	 #, tmp269
	movq	%r10, -8(%r8,%rcx)	 # tmp269,
	leaq	24(%r15), %r10	 #, tmp270
	andq	$-8, %r10	 #, tmp270
	subq	%r10, %r8	 # tmp270, _155
	addl	%r8d, %edx	 # _155, tmp241
	subq	%r8, %rsi	 # _155, _157
	andl	$-8, %edx	 #, tmp241
	cmpl	$8, %edx	 #, tmp241
	jb	.L451	 #,
	andl	$-8, %edx	 #, tmp272
	xorl	%ecx, %ecx	 # tmp271
.L381:
	movl	%ecx, %r8d	 # tmp271, tmp273
	addl	$8, %ecx	 #, tmp271
	movq	(%rsi,%r8), %r11	 #, tmp274
	movq	%r11, (%r10,%r8)	 # tmp274,
	cmpl	%edx, %ecx	 # tmp272, tmp271
	jb	.L381	 #,
	jmp	.L451	 #
.L420:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:69: 	  if (__value < __b3) return __n + 2;
	movl	$2, %ebp	 #, __pos
	movl	$3, %edx	 #, _375
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:69: 	  if (__value < __b3) return __n + 2;
	jmp	.L368	 #
.L453:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/charconv.h:70: 	  if (__value < __b4) return __n + 3;
	movl	$3, %ebp	 #, __pos
	movl	$4, %edx	 #, _375
	jmp	.L368	 #
.L445:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/char_traits.h:445: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movl	%edx, %edx	 # tmp240, tmp256
	movzwl	-2(%r9,%rdx), %ecx	 #, tmp261
	movw	%cx, -2(%r8,%rdx)	 # tmp261,
	movq	8(%rax), %rcx	 # MEM[(const struct basic_string *)_148]._M_string_length, MEM[(const struct basic_string *)_148]._M_string_length
	jmp	.L383	 #
.L461:
	movl	16(%rax), %ecx	 #, tmp245
	movl	%edx, %edx	 # tmp240, tmp246
	movl	%ecx, 16(%r15)	 # tmp245,
	movl	-4(%r9,%rdx), %ecx	 #, tmp252
	movl	%ecx, -4(%r8,%rdx)	 # tmp252,
	movq	8(%rax), %rcx	 # MEM[(const struct basic_string *)_148]._M_string_length, MEM[(const struct basic_string *)_148]._M_string_length
	jmp	.L383	 #
.L454:
	movl	$1, %eax	 #, __n
	movl	$5, %ebp	 #, __pos
	jmp	.L371	 #
.L399:
 # data.hpp:152:         assertm(i >= 0 && i < size(), "Index out of bounds!");
	movl	$152, %r8d	 #,
	leaq	.LC0(%rip), %rdx	 #, tmp358
	leaq	.LC5(%rip), %rcx	 #, tmp359
.LEHB36:
	call	*__imp__assert(%rip)	 #
.LEHE36:
.L428:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_ios.h:282:       ~basic_ios() { }
	movq	%rax, %rbx	 # tmp456, tmp325
	jmp	.L390	 #
.L425:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:803:       { _M_dispose(); }
	movq	%rax, %rbx	 # tmp466, tmp431
	jmp	.L418	 #
.L389:
 # C:/Program Files/Perl/c/include/c++/13.1.0/sstream:79:     class basic_stringbuf : public basic_streambuf<_CharT, _Traits>
	movq	.refptr._ZTVNSt7__cxx1115basic_stringbufIcSt11char_traitsIcESaIcEEE(%rip), %rax	 #, tmp521
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:803:       { _M_dispose(); }
	leaq	208(%rsp), %rcx	 #, tmp315
 # C:/Program Files/Perl/c/include/c++/13.1.0/sstream:79:     class basic_stringbuf : public basic_streambuf<_CharT, _Traits>
	addq	$16, %rax	 #, tmp313
	movq	%rax, 136(%rsp)	 # tmp313, MEM[(struct basic_stringbuf *)_402].D.81137._vptr.basic_streambuf
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:803:       { _M_dispose(); }
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv	 #
 # C:/Program Files/Perl/c/include/c++/13.1.0/streambuf:205:       { }
	movq	.refptr._ZTVSt15basic_streambufIcSt11char_traitsIcEE(%rip), %rax	 #, tmp522
	movq	72(%rsp), %rcx	 # %sfp,
	addq	$16, %rax	 #, tmp317
	movq	%rax, 136(%rsp)	 # tmp317, MEM[(struct basic_streambuf *)_402]._vptr.basic_streambuf
	call	_ZNSt6localeD1Ev	 #
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:95:       ~basic_ostream() { }
	movq	56(%rsp), %rax	 # %sfp, _165
	movq	64(%rsp), %rdi	 # %sfp, _169
	movq	%rax, 128(%rsp)	 # _165, MEM[(struct basic_ostream *)_402]._vptr.basic_ostream
	movq	-24(%rax), %rax	 # MEM[(long long int *)_165 + -24B], MEM[(long long int *)_165 + -24B]
	movq	%rdi, 128(%rsp,%rax)	 # _169, MEM[(struct basic_ios *)_177].D.48244._vptr.ios_base
.L390:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_ios.h:282:       ~basic_ios() { }
	movq	.refptr._ZTVSt9basic_iosIcSt11char_traitsIcEE(%rip), %rax	 #, tmp526
	movq	80(%rsp), %rcx	 # %sfp,
	addq	$16, %rax	 #, tmp328
	movq	%rax, 240(%rsp)	 # tmp328, MEM[(struct basic_ios *)_402].D.48244._vptr.ios_base
	call	_ZNSt8ios_baseD2Ev	 #
	movq	%rbx, %rcx	 # tmp325,
.LEHB37:
	call	_Unwind_Resume	 #
.L418:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:803:       { _M_dispose(); }
	movq	%rdi, %rcx	 # tmp444,
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv	 #
	movq	%rbx, %rcx	 # tmp431,
	call	_Unwind_Resume	 #
.LEHE37:
.L429:
 # C:/Program Files/Perl/c/include/c++/13.1.0/sstream:79:     class basic_stringbuf : public basic_streambuf<_CharT, _Traits>
	movq	%rax, %rbx	 # tmp455, tmp321
	jmp	.L389	 #
.L457:
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	movl	$16, %ecx	 #,
	call	__cxa_allocate_exception	 #
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	leaq	.LC4(%rip), %rdx	 #, tmp333
	movq	%rax, %rcx	 # _179,
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	movq	%rax, %rsi	 # tmp457, _179
.LEHB38:
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	call	_ZNSt12out_of_rangeC1EPKc	 #
.LEHE38:
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	leaq	_ZNSt12out_of_rangeD1Ev(%rip), %r8	 #,
	leaq	_ZTISt12out_of_range(%rip), %rdx	 #, tmp334
	movq	%rsi, %rcx	 # _179,
.LEHB39:
	call	__cxa_throw	 #
.LEHE39:
.L430:
.L452:
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	movq	%rax, %rbx	 # tmp464, tmp398
	jmp	.L411	 #
.L426:
 # array.hpp:1011:     }
	movq	%rax, %rbx	 # tmp468, tmp340
	jmp	.L395	 #
.L459:
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	movl	$16, %ecx	 #,
	call	__cxa_allocate_exception	 #
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	leaq	.LC4(%rip), %rdx	 #, tmp365
	movq	%rax, %rcx	 # _193,
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	movq	%rax, %rsi	 # tmp461, _193
.LEHB40:
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	call	_ZNSt12out_of_rangeC1EPKc	 #
.LEHE40:
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	leaq	_ZNSt12out_of_rangeD1Ev(%rip), %r8	 #,
	leaq	_ZTISt12out_of_range(%rip), %rdx	 #, tmp366
	movq	%rsi, %rcx	 # _193,
.LEHB41:
	call	__cxa_throw	 #
.LEHE41:
.L411:
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	movq	%rsi, %rcx	 # _205,
	call	__cxa_free_exception	 #
.L395:
 # array.hpp:1011:     }
	movq	%rbp, %rcx	 # tmp446,
	call	_ZNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEED1Ev	 #
	movq	%rbx, %rcx	 # tmp340,
.LEHB42:
	call	_Unwind_Resume	 #
.LEHE42:
.L432:
	jmp	.L452	 #
.L458:
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	movl	$16, %ecx	 #,
	call	__cxa_allocate_exception	 #
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	leaq	.LC4(%rip), %rdx	 #, tmp392
	movq	%rax, %rcx	 # _205,
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	movq	%rax, %rsi	 # tmp463, _205
.LEHB43:
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	call	_ZNSt12out_of_rangeC1EPKc	 #
.LEHE43:
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	leaq	_ZNSt12out_of_rangeD1Ev(%rip), %r8	 #,
	leaq	_ZTISt12out_of_range(%rip), %rdx	 #, tmp393
	movq	%rsi, %rcx	 # _205,
.LEHB44:
	call	__cxa_throw	 #
.LEHE44:
.L456:
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	movl	$16, %ecx	 #,
	call	__cxa_allocate_exception	 #
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	leaq	.LC4(%rip), %rdx	 #, tmp347
	movq	%rax, %rcx	 # _184,
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	movq	%rax, %rsi	 # tmp459, _184
.LEHB45:
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	call	_ZNSt12out_of_rangeC1EPKc	 #
.LEHE45:
 # stack_buffer.hpp:134:             throw std::out_of_range("The index is out of range.");
	leaq	_ZNSt12out_of_rangeD1Ev(%rip), %r8	 #,
	leaq	_ZTISt12out_of_range(%rip), %rdx	 #, tmp348
	movq	%rsi, %rcx	 # _184,
.LEHB46:
	call	__cxa_throw	 #
.LEHE46:
.L433:
	jmp	.L452	 #
.L431:
	jmp	.L452	 #
.L427:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:803:       { _M_dispose(); }
	movq	%rax, %rbx	 # tmp467, tmp433
	jmp	.L419	 #
.L434:
	movq	%rax, %rbx	 # tmp465, tmp416
	jmp	.L417	 #
.L419:
	movq	%rdi, %rcx	 # tmp444,
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv	 #
	jmp	.L395	 #
.L417:
	movq	%r15, %rcx	 # <retval>,
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv	 #
	jmp	.L395	 #
	.seh_handler	__gxx_personality_seh0, @unwind, @except
	.seh_handlerdata
.LLSDA6327:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE6327-.LLSDACSB6327
.LLSDACSB6327:
	.uleb128 .LEHB30-.LFB6327
	.uleb128 .LEHE30-.LEHB30
	.uleb128 .L425-.LFB6327
	.uleb128 0
	.uleb128 .LEHB31-.LFB6327
	.uleb128 .LEHE31-.LEHB31
	.uleb128 .L428-.LFB6327
	.uleb128 0
	.uleb128 .LEHB32-.LFB6327
	.uleb128 .LEHE32-.LEHB32
	.uleb128 .L429-.LFB6327
	.uleb128 0
	.uleb128 .LEHB33-.LFB6327
	.uleb128 .LEHE33-.LEHB33
	.uleb128 .L426-.LFB6327
	.uleb128 0
	.uleb128 .LEHB34-.LFB6327
	.uleb128 .LEHE34-.LEHB34
	.uleb128 .L427-.LFB6327
	.uleb128 0
	.uleb128 .LEHB35-.LFB6327
	.uleb128 .LEHE35-.LEHB35
	.uleb128 .L434-.LFB6327
	.uleb128 0
	.uleb128 .LEHB36-.LFB6327
	.uleb128 .LEHE36-.LEHB36
	.uleb128 .L426-.LFB6327
	.uleb128 0
	.uleb128 .LEHB37-.LFB6327
	.uleb128 .LEHE37-.LEHB37
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB38-.LFB6327
	.uleb128 .LEHE38-.LEHB38
	.uleb128 .L430-.LFB6327
	.uleb128 0
	.uleb128 .LEHB39-.LFB6327
	.uleb128 .LEHE39-.LEHB39
	.uleb128 .L426-.LFB6327
	.uleb128 0
	.uleb128 .LEHB40-.LFB6327
	.uleb128 .LEHE40-.LEHB40
	.uleb128 .L432-.LFB6327
	.uleb128 0
	.uleb128 .LEHB41-.LFB6327
	.uleb128 .LEHE41-.LEHB41
	.uleb128 .L426-.LFB6327
	.uleb128 0
	.uleb128 .LEHB42-.LFB6327
	.uleb128 .LEHE42-.LEHB42
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB43-.LFB6327
	.uleb128 .LEHE43-.LEHB43
	.uleb128 .L433-.LFB6327
	.uleb128 0
	.uleb128 .LEHB44-.LFB6327
	.uleb128 .LEHE44-.LEHB44
	.uleb128 .L426-.LFB6327
	.uleb128 0
	.uleb128 .LEHB45-.LFB6327
	.uleb128 .LEHE45-.LEHB45
	.uleb128 .L431-.LFB6327
	.uleb128 0
	.uleb128 .LEHB46-.LFB6327
	.uleb128 .LEHE46-.LEHB46
	.uleb128 .L426-.LFB6327
	.uleb128 0
.LLSDACSE6327:
	.section	.text$_ZNK5ArrayIdE9to_stringB5cxx11Ev,"x"
	.linkonce discard
	.seh_endproc
	.section	.text$_ZlsIdERSoS0_RK5ArrayIT_E,"x"
	.linkonce discard
	.p2align 4
	.globl	_ZlsIdERSoS0_RK5ArrayIT_E
	.def	_ZlsIdERSoS0_RK5ArrayIT_E;	.scl	2;	.type	32;	.endef
	.seh_proc	_ZlsIdERSoS0_RK5ArrayIT_E
_ZlsIdERSoS0_RK5ArrayIT_E:
.LFB6099:
	pushq	%rsi	 #
	.seh_pushreg	%rsi
	pushq	%rbx	 #
	.seh_pushreg	%rbx
	subq	$72, %rsp	 #,
	.seh_stackalloc	72
	.seh_endprologue
 # array.hpp:1017:     return s << x.to_string();
	leaq	32(%rsp), %rsi	 #, tmp102
 # array.hpp:1015: std::ostream &operator<<(std::ostream &s, const Array<T> &x)
	movq	%rcx, %rbx	 # tmp103, s
 # array.hpp:1017:     return s << x.to_string();
	movq	%rsi, %rcx	 # tmp102,
.LEHB47:
	call	_ZNK5ArrayIdE9to_stringB5cxx11Ev	 #
.LEHE47:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:4036:       return __ostream_insert(__os, __str.data(), __str.size());
	movq	40(%rsp), %r8	 # D.119226._M_string_length,
	movq	32(%rsp), %rdx	 # D.119226._M_dataplus._M_p,
	movq	%rbx, %rcx	 # s,
.LEHB48:
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x	 #
.LEHE48:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:234:       { return _M_dataplus._M_p; }
	movq	32(%rsp), %rcx	 # D.119226._M_dataplus._M_p, _15
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:4036:       return __ostream_insert(__os, __str.data(), __str.size());
	movq	%rax, %rbx	 # tmp105, <retval>
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:275: 	if (_M_data() == _M_local_data())
	leaq	48(%rsp), %rax	 #, tmp96
	cmpq	%rax, %rcx	 # tmp96, _15
	je	.L462	 #,
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:300:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	48(%rsp), %rax	 # D.119226.D.29453._M_allocated_capacity, tmp108
	leaq	1(%rax), %rdx	 #, tmp97
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/new_allocator.h:168: 	_GLIBCXX_OPERATOR_DELETE(_GLIBCXX_SIZED_DEALLOC(__p, __n));
	call	_ZdlPvy	 #
.L462:
 # array.hpp:1018: }
	movq	%rbx, %rax	 # <retval>,
	addq	$72, %rsp	 #,
	popq	%rbx	 #
	popq	%rsi	 #
	ret	
.L465:
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/basic_string.h:803:       { _M_dispose(); }
	movq	%rax, %rbx	 # tmp106, tmp101
.L464:
	movq	%rsi, %rcx	 # tmp102,
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv	 #
	movq	%rbx, %rcx	 # tmp101,
.LEHB49:
	call	_Unwind_Resume	 #
	nop	
.LEHE49:
	.seh_handler	__gxx_personality_seh0, @unwind, @except
	.seh_handlerdata
.LLSDA6099:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE6099-.LLSDACSB6099
.LLSDACSB6099:
	.uleb128 .LEHB47-.LFB6099
	.uleb128 .LEHE47-.LEHB47
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB48-.LFB6099
	.uleb128 .LEHE48-.LEHB48
	.uleb128 .L465-.LFB6099
	.uleb128 0
	.uleb128 .LEHB49-.LFB6099
	.uleb128 .LEHE49-.LEHB49
	.uleb128 0
	.uleb128 0
.LLSDACSE6099:
	.section	.text$_ZlsIdERSoS0_RK5ArrayIT_E,"x"
	.linkonce discard
	.seh_endproc
	.def	__main;	.scl	2;	.type	32;	.endef
	.section	.text.unlikely,"x"
.LCOLDB14:
	.section	.text.startup,"x"
.LHOTB14:
	.p2align 4
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
.LFB5724:
	pushq	%r12	 #
	.seh_pushreg	%r12
	pushq	%rbp	 #
	.seh_pushreg	%rbp
	pushq	%rdi	 #
	.seh_pushreg	%rdi
	pushq	%rsi	 #
	.seh_pushreg	%rsi
	pushq	%rbx	 #
	.seh_pushreg	%rbx
	subq	$512, %rsp	 #,
	.seh_stackalloc	512
	.seh_endprologue
 # main.cpp:25: {
	call	__main	 #
 # main.cpp:28:     auto x = randomArrayGenerator.uniformInteger<double>({100,100}, -1, 2);
	leaq	60(%rsp), %rbp	 #, tmp97
	leaq	64(%rsp), %rsi	 #, tmp131
	movl	$2, 32(%rsp)	 #,
 # stack_buffer.hpp:63:             mBuffer[mSize++] = value;
	movq	.LC13(%rip), %rdi	 #, tmp94
 # main.cpp:28:     auto x = randomArrayGenerator.uniformInteger<double>({100,100}, -1, 2);
	movl	$-1, %r9d	 #,
	movq	%rbp, %rdx	 # tmp97,
	movq	%rsi, %rcx	 # tmp131,
	leaq	400(%rsp), %rbx	 #, tmp132
 # C:/Program Files/Perl/c/include/c++/13.1.0/bits/random.tcc:123: 	_M_x = 1;
	movl	$1, 60(%rsp)	 #, MEM[(struct linear_congruential_engine *)&randomArrayGenerator]._M_x
 # main.cpp:28:     auto x = randomArrayGenerator.uniformInteger<double>({100,100}, -1, 2);
	movq	%rbx, %r8	 # tmp132,
 # stack_buffer.hpp:63:             mBuffer[mSize++] = value;
	movq	%rdi, 400(%rsp)	 # tmp94, MEM <vector(2) long int> [(long int *)_7]
 # stack_buffer.hpp:63:             mBuffer[mSize++] = value;
	movq	$2, 432(%rsp)	 #, MEM[(struct StackBuffer *)_7].mSize
.LEHB50:
 # main.cpp:28:     auto x = randomArrayGenerator.uniformInteger<double>({100,100}, -1, 2);
	call	_ZN20RandomArrayGenerator14uniformIntegerIdEE5ArrayIT_ERK11StackBufferIlLy8EEll	 #
.LEHE50:
 # main.cpp:29:     auto y = randomArrayGenerator.uniformInteger<double>({100,100}, -1, 2);
	movl	$2, 32(%rsp)	 #,
	movq	%rbx, %r8	 # tmp132,
	movq	%rbp, %rdx	 # tmp97,
 # stack_buffer.hpp:63:             mBuffer[mSize++] = value;
	movq	%rdi, 400(%rsp)	 # tmp94, MEM <vector(2) long int> [(long int *)_7]
 # main.cpp:29:     auto y = randomArrayGenerator.uniformInteger<double>({100,100}, -1, 2);
	leaq	176(%rsp), %rdi	 #, tmp133
	movl	$-1, %r9d	 #,
 # stack_buffer.hpp:63:             mBuffer[mSize++] = value;
	movq	$2, 432(%rsp)	 #, MEM[(struct StackBuffer *)_7].mSize
 # main.cpp:29:     auto y = randomArrayGenerator.uniformInteger<double>({100,100}, -1, 2);
	movq	%rdi, %rcx	 # tmp133,
.LEHB51:
	call	_ZN20RandomArrayGenerator14uniformIntegerIdEE5ArrayIT_ERK11StackBufferIlLy8EEll	 #
.LEHE51:
 # main.cpp:30:     auto a = x+y;
	leaq	288(%rsp), %rbp	 #, tmp130
	movq	%rdi, %r8	 # tmp133,
	movq	%rsi, %rdx	 # tmp131,
	movq	%rbp, %rcx	 # tmp130,
.LEHB52:
	call	_ZNK5ArrayIdEplERKS0_	 #
.LEHE52:
 # main.cpp:31:     auto b = x*y;
	movq	%rdi, %r8	 # tmp133,
	movq	%rsi, %rdx	 # tmp131,
	movq	%rbx, %rcx	 # tmp132,
.LEHB53:
	call	_ZNK5ArrayIdEmlERKS0_	 #
.LEHE53:
 # main.cpp:32:     std::cout << a << std::endl;
	movq	.refptr._ZSt4cout(%rip), %r12	 #, tmp129
	movq	%rbp, %rdx	 # tmp130,
	movq	%r12, %rcx	 # tmp129,
.LEHB54:
	call	_ZlsIdERSoS0_RK5ArrayIT_E	 #
	movq	%rax, %rcx	 # tmp134, _19
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:115: 	return __pf(*this);
	call	_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_.isra.0	 #
 # main.cpp:33:     std::cout << b << std::endl;
	movq	%rbx, %rdx	 # tmp132,
	movq	%r12, %rcx	 # tmp129,
	call	_ZlsIdERSoS0_RK5ArrayIT_E	 #
	movq	%rax, %rcx	 # tmp135, _21
 # C:/Program Files/Perl/c/include/c++/13.1.0/ostream:115: 	return __pf(*this);
	call	_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_.isra.0	 #
.LEHE54:
 # data.hpp:167:         if (mControl != nullptr)
	cmpq	$0, 408(%rsp)	 #, MEM[(struct Data *)_7].mControl
	je	.L467	 #,
 # data.hpp:168:             release();
	movq	%rbx, %rcx	 # tmp132,
	call	_ZN4DataIdE7releaseEv	 #
.L467:
 # data.hpp:167:         if (mControl != nullptr)
	cmpq	$0, 296(%rsp)	 #, MEM[(struct Data *)&a].mControl
	je	.L468	 #,
 # data.hpp:168:             release();
	movq	%rbp, %rcx	 # tmp130,
	call	_ZN4DataIdE7releaseEv	 #
.L468:
 # data.hpp:167:         if (mControl != nullptr)
	cmpq	$0, 184(%rsp)	 #, MEM[(struct Data *)&y].mControl
	je	.L469	 #,
 # data.hpp:168:             release();
	movq	%rdi, %rcx	 # tmp133,
	call	_ZN4DataIdE7releaseEv	 #
.L469:
 # data.hpp:167:         if (mControl != nullptr)
	cmpq	$0, 72(%rsp)	 #, MEM[(struct Data *)&x].mControl
	je	.L483	 #,
 # data.hpp:168:             release();
	movq	%rsi, %rcx	 # tmp131,
	call	_ZN4DataIdE7releaseEv	 #
.L483:
 # main.cpp:49: }
	xorl	%eax, %eax	 #
	addq	$512, %rsp	 #,
	popq	%rbx	 #
	popq	%rsi	 #
	popq	%rdi	 #
	popq	%rbp	 #
	popq	%r12	 #
	ret	
.L479:
 # data.hpp:167:         if (mControl != nullptr)
	movq	%rax, %rbx	 # tmp139, tmp126
	jmp	.L477	 #
.L482:
	movq	%rax, %r12	 # tmp136, tmp119
	jmp	.L471	 #
.L481:
	movq	%rax, %rbx	 # tmp137, tmp118
	jmp	.L473	 #
.L480:
	movq	%rax, %rbx	 # tmp138, tmp123
	jmp	.L475	 #
	.seh_handler	__gxx_personality_seh0, @unwind, @except
	.seh_handlerdata
.LLSDA5724:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE5724-.LLSDACSB5724
.LLSDACSB5724:
	.uleb128 .LEHB50-.LFB5724
	.uleb128 .LEHE50-.LEHB50
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB51-.LFB5724
	.uleb128 .LEHE51-.LEHB51
	.uleb128 .L479-.LFB5724
	.uleb128 0
	.uleb128 .LEHB52-.LFB5724
	.uleb128 .LEHE52-.LEHB52
	.uleb128 .L480-.LFB5724
	.uleb128 0
	.uleb128 .LEHB53-.LFB5724
	.uleb128 .LEHE53-.LEHB53
	.uleb128 .L481-.LFB5724
	.uleb128 0
	.uleb128 .LEHB54-.LFB5724
	.uleb128 .LEHE54-.LEHB54
	.uleb128 .L482-.LFB5724
	.uleb128 0
.LLSDACSE5724:
	.section	.text.startup,"x"
	.seh_endproc
	.section	.text.unlikely,"x"
	.def	main.cold;	.scl	3;	.type	32;	.endef
	.seh_proc	main.cold
	.seh_stackalloc	552
	.seh_savereg	%rbx, 512
	.seh_savereg	%rsi, 520
	.seh_savereg	%rdi, 528
	.seh_savereg	%rbp, 536
	.seh_savereg	%r12, 544
	.seh_endprologue
main.cold:
.L471:
	cmpq	$0, 408(%rsp)	 #, MEM[(struct Data *)_7].mControl
	jne	.L484	 #,
.L472:
	movq	%r12, %rbx	 # tmp119, tmp118
.L473:
	cmpq	$0, 296(%rsp)	 #, MEM[(struct Data *)&a].mControl
	je	.L475	 #,
 # data.hpp:168:             release();
	movq	%rbp, %rcx	 # tmp130,
	call	_ZN4DataIdE7releaseEv	 #
.L475:
 # data.hpp:167:         if (mControl != nullptr)
	cmpq	$0, 184(%rsp)	 #, MEM[(struct Data *)&y].mControl
	je	.L477	 #,
 # data.hpp:168:             release();
	movq	%rdi, %rcx	 # tmp133,
	call	_ZN4DataIdE7releaseEv	 #
.L477:
 # data.hpp:167:         if (mControl != nullptr)
	cmpq	$0, 72(%rsp)	 #, MEM[(struct Data *)&x].mControl
	je	.L478	 #,
 # data.hpp:168:             release();
	movq	%rsi, %rcx	 # tmp131,
	call	_ZN4DataIdE7releaseEv	 #
.L478:
	movq	%rbx, %rcx	 # tmp126,
.LEHB55:
	call	_Unwind_Resume	 #
.LEHE55:
.L484:
	movq	%rbx, %rcx	 # tmp132,
	call	_ZN4DataIdE7releaseEv	 #
	jmp	.L472	 #
	.seh_handler	__gxx_personality_seh0, @unwind, @except
	.seh_handlerdata
.LLSDAC5724:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSEC5724-.LLSDACSBC5724
.LLSDACSBC5724:
	.uleb128 .LEHB55-.LCOLDB14
	.uleb128 .LEHE55-.LEHB55
	.uleb128 0
	.uleb128 0
.LLSDACSEC5724:
	.section	.text.unlikely,"x"
	.section	.text.startup,"x"
	.section	.text.unlikely,"x"
	.seh_endproc
.LCOLDE14:
	.section	.text.startup,"x"
.LHOTE14:
	.globl	_ZTSSt9exception
	.section	.rdata$_ZTSSt9exception,"dr"
	.linkonce same_size
	.align 8
_ZTSSt9exception:
	.ascii "St9exception\0"
	.globl	_ZTISt9exception
	.section	.rdata$_ZTISt9exception,"dr"
	.linkonce same_size
	.align 8
_ZTISt9exception:
 # <anonymous>:
 # <anonymous>:
	.quad	_ZTVN10__cxxabiv117__class_type_infoE+16
 # <anonymous>:
	.quad	_ZTSSt9exception
	.globl	_ZTSSt11logic_error
	.section	.rdata$_ZTSSt11logic_error,"dr"
	.linkonce same_size
	.align 16
_ZTSSt11logic_error:
	.ascii "St11logic_error\0"
	.globl	_ZTISt11logic_error
	.section	.rdata$_ZTISt11logic_error,"dr"
	.linkonce same_size
	.align 8
_ZTISt11logic_error:
 # <anonymous>:
 # <anonymous>:
	.quad	_ZTVN10__cxxabiv120__si_class_type_infoE+16
 # <anonymous>:
	.quad	_ZTSSt11logic_error
 # <anonymous>:
	.quad	_ZTISt9exception
	.globl	_ZTSSt16invalid_argument
	.section	.rdata$_ZTSSt16invalid_argument,"dr"
	.linkonce same_size
	.align 16
_ZTSSt16invalid_argument:
	.ascii "St16invalid_argument\0"
	.globl	_ZTISt16invalid_argument
	.section	.rdata$_ZTISt16invalid_argument,"dr"
	.linkonce same_size
	.align 8
_ZTISt16invalid_argument:
 # <anonymous>:
 # <anonymous>:
	.quad	_ZTVN10__cxxabiv120__si_class_type_infoE+16
 # <anonymous>:
	.quad	_ZTSSt16invalid_argument
 # <anonymous>:
	.quad	_ZTISt11logic_error
	.globl	_ZTSSt12out_of_range
	.section	.rdata$_ZTSSt12out_of_range,"dr"
	.linkonce same_size
	.align 16
_ZTSSt12out_of_range:
	.ascii "St12out_of_range\0"
	.globl	_ZTISt12out_of_range
	.section	.rdata$_ZTISt12out_of_range,"dr"
	.linkonce same_size
	.align 8
_ZTISt12out_of_range:
 # <anonymous>:
 # <anonymous>:
	.quad	_ZTVN10__cxxabiv120__si_class_type_infoE+16
 # <anonymous>:
	.quad	_ZTSSt12out_of_range
 # <anonymous>:
	.quad	_ZTISt11logic_error
	.globl	_ZTSZNK5ArrayIdEmlERKS0_EUlddE_
	.section	.rdata$_ZTSZNK5ArrayIdEmlERKS0_EUlddE_,"dr"
	.linkonce same_size
	.align 16
_ZTSZNK5ArrayIdEmlERKS0_EUlddE_:
	.ascii "ZNK5ArrayIdEmlERKS0_EUlddE_\0"
	.globl	_ZTIZNK5ArrayIdEmlERKS0_EUlddE_
	.section	.rdata$_ZTIZNK5ArrayIdEmlERKS0_EUlddE_,"dr"
	.linkonce same_size
	.align 8
_ZTIZNK5ArrayIdEmlERKS0_EUlddE_:
 # <anonymous>:
 # <anonymous>:
	.quad	_ZTVN10__cxxabiv117__class_type_infoE+16
 # <anonymous>:
	.quad	_ZTSZNK5ArrayIdEmlERKS0_EUlddE_
	.section .rdata,"dr"
	.align 8
.LC13:
	.long	100
	.long	100
	.ident	"GCC: (MinGW-W64 x86_64-msvcrt-posix-seh, built by Brecht Sanders) 13.1.0"
	.def	_ZNSo3putEc;	.scl	2;	.type	32;	.endef
	.def	_ZNSo5flushEv;	.scl	2;	.type	32;	.endef
	.def	_ZNKSt5ctypeIcE13_M_widen_initEv;	.scl	2;	.type	32;	.endef
	.def	_ZSt16__throw_bad_castv;	.scl	2;	.type	32;	.endef
	.def	_ZdaPv;	.scl	2;	.type	32;	.endef
	.def	_ZdlPvy;	.scl	2;	.type	32;	.endef
	.def	_Znay;	.scl	2;	.type	32;	.endef
	.def	_Znwy;	.scl	2;	.type	32;	.endef
	.def	memmove;	.scl	2;	.type	32;	.endef
	.def	__cxa_allocate_exception;	.scl	2;	.type	32;	.endef
	.def	_ZNSt12out_of_rangeC1EPKc;	.scl	2;	.type	32;	.endef
	.def	_ZNSt12out_of_rangeD1Ev;	.scl	2;	.type	32;	.endef
	.def	__cxa_throw;	.scl	2;	.type	32;	.endef
	.def	_Unwind_Resume;	.scl	2;	.type	32;	.endef
	.def	__cxa_throw_bad_array_new_length;	.scl	2;	.type	32;	.endef
	.def	__cxa_free_exception;	.scl	2;	.type	32;	.endef
	.def	_ZNSt16invalid_argumentC1EPKc;	.scl	2;	.type	32;	.endef
	.def	_ZNSt16invalid_argumentD1Ev;	.scl	2;	.type	32;	.endef
	.def	_ZSt25__throw_bad_function_callv;	.scl	2;	.type	32;	.endef
	.def	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE12_M_constructEyc;	.scl	2;	.type	32;	.endef
	.def	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_replaceEyyPKcy;	.scl	2;	.type	32;	.endef
	.def	_ZNSt8ios_baseC2Ev;	.scl	2;	.type	32;	.endef
	.def	_ZNSt9basic_iosIcSt11char_traitsIcEE4initEPSt15basic_streambufIcS1_E;	.scl	2;	.type	32;	.endef
	.def	_ZNSt6localeC1Ev;	.scl	2;	.type	32;	.endef
	.def	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x;	.scl	2;	.type	32;	.endef
	.def	_ZNSo9_M_insertIdEERSoT_;	.scl	2;	.type	32;	.endef
	.def	_ZNSt6localeD1Ev;	.scl	2;	.type	32;	.endef
	.def	_ZNSt8ios_baseD2Ev;	.scl	2;	.type	32;	.endef
	.def	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_assignERKS4_;	.scl	2;	.type	32;	.endef
	.def	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv;	.scl	2;	.type	32;	.endef
	.def	_ZNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEED1Ev;	.scl	2;	.type	32;	.endef
	.section	.rdata$.refptr._ZSt4cout, "dr"
	.globl	.refptr._ZSt4cout
	.linkonce	discard
.refptr._ZSt4cout:
	.quad	_ZSt4cout
	.section	.rdata$.refptr._ZTVNSt7__cxx1115basic_stringbufIcSt11char_traitsIcESaIcEEE, "dr"
	.globl	.refptr._ZTVNSt7__cxx1115basic_stringbufIcSt11char_traitsIcESaIcEEE
	.linkonce	discard
.refptr._ZTVNSt7__cxx1115basic_stringbufIcSt11char_traitsIcESaIcEEE:
	.quad	_ZTVNSt7__cxx1115basic_stringbufIcSt11char_traitsIcESaIcEEE
	.section	.rdata$.refptr._ZTVSt15basic_streambufIcSt11char_traitsIcEE, "dr"
	.globl	.refptr._ZTVSt15basic_streambufIcSt11char_traitsIcEE
	.linkonce	discard
.refptr._ZTVSt15basic_streambufIcSt11char_traitsIcEE:
	.quad	_ZTVSt15basic_streambufIcSt11char_traitsIcEE
	.section	.rdata$.refptr._ZTVNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE, "dr"
	.globl	.refptr._ZTVNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE
	.linkonce	discard
.refptr._ZTVNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE:
	.quad	_ZTVNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE
	.section	.rdata$.refptr._ZTTNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE, "dr"
	.globl	.refptr._ZTTNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE
	.linkonce	discard
.refptr._ZTTNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE:
	.quad	_ZTTNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE
	.section	.rdata$.refptr._ZTVSt9basic_iosIcSt11char_traitsIcEE, "dr"
	.globl	.refptr._ZTVSt9basic_iosIcSt11char_traitsIcEE
	.linkonce	discard
.refptr._ZTVSt9basic_iosIcSt11char_traitsIcEE:
	.quad	_ZTVSt9basic_iosIcSt11char_traitsIcEE
	.section	.rdata$.refptr._ZNSt12out_of_rangeD1Ev, "dr"
	.globl	.refptr._ZNSt12out_of_rangeD1Ev
	.linkonce	discard
.refptr._ZNSt12out_of_rangeD1Ev:
	.quad	_ZNSt12out_of_rangeD1Ev
	.section	.rdata$.refptr._ZNSt16invalid_argumentD1Ev, "dr"
	.globl	.refptr._ZNSt16invalid_argumentD1Ev
	.linkonce	discard
.refptr._ZNSt16invalid_argumentD1Ev:
	.quad	_ZNSt16invalid_argumentD1Ev
