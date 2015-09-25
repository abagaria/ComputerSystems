0000000000400fbc <phase_2>:
  400fbc:	53                   	push   %rbx
  400fbd:	48 83 ec 20          	sub    $0x20,%rsp
  400fc1:	48 89 e6             	mov    %rsp,%rsi
  400fc4:	e8 cd 07 00 00       	callq  401796 <read_six_numbers>
  400fc9:	83 3c 24 01          	cmpl   $0x1,(%rsp)
  400fcd:	74 05                	je     400fd4 <phase_2+0x18>
  400fcf:	e8 8c 07 00 00       	callq  401760 <explode_bomb>
  400fd4:	bb 01 00 00 00       	mov    $0x1,%ebx
  400fd9:	eb 1a                	jmp    400ff5 <phase_2+0x39>
  400fdb:	48 63 d3             	movslq %ebx,%rdx
  400fde:	8d 43 ff             	lea    -0x1(%rbx),%eax
  400fe1:	48 98                	cltq   
  400fe3:	8b 04 84             	mov    (%rsp,%rax,4),%eax
  400fe6:	01 c0                	add    %eax,%eax
  400fe8:	39 04 94             	cmp    %eax,(%rsp,%rdx,4)
  400feb:	74 05                	je     400ff2 <phase_2+0x36>
  400fed:	e8 6e 07 00 00       	callq  401760 <explode_bomb>
  400ff2:	83 c3 01             	add    $0x1,%ebx
  400ff5:	83 fb 05             	cmp    $0x5,%ebx
  400ff8:	7e e1                	jle    400fdb <phase_2+0x1f>
  400ffa:	48 83 c4 20          	add    $0x20,%rsp
  400ffe:	5b                   	pop    %rbx
  400fff:	c3                   	retq   