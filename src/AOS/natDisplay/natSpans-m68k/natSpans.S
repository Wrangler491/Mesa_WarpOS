




	.equ	x_ofs,?
	.equ	y_ofs,?
	.equ	rgba_ofs,?
	.equ	mask_ofs,?

	.equ	DriverCtx_ofs,?

	.equ	rp_ofs,?
	.equ	il_ofs,?
	.equ	FixedX_ofs,?
	.equ	FixedY_ofs,?
	
	.equ	amesa,a6
	.equ	gp,a5
	.equ	rp,a5
	.equ	il,a4
	.equ	rgba,a3
	.equ	mask,a2

	.equ	n,d7
	.equ	x,d6
	.equ	y,d5

	lea	pc@(GetPen),gp
	movel	sp@(ctx_ofs),a0
	movel	a0@(DriverCtx_ofs),amesa
	movel	amesa@(il_ofs),il
	movel	sp@(n_ofs),n
	movel	sp@(x_ofs),x
	movel	sp@(y_ofs),y
	addl	amesa@(FixedX_ofs),x
	addl	amesa@(FixedX_ofs),y
	movel	sp@(rgba_ofs),rgba
	movel	sp@(mask_ofs),d0
	bnes	.mask
.nomask	subql	#1,n
	blts	.nend
.nloop	moveb	rgba@+,d0
	moveb	rgba@+,d1
	moveb	rgba@+,d2
	addql	#1,rgba
	jsr	gp@
	moveb	d0,il@+
	dbra	n,.nloop
.nend	movel	amesa@(rp_ofs),?
	movel	x,?
	movel	y,?
	movel	n,?
	movel	amesa@(il_ofs),?
	movel	amesa@(temprp_ofs),?
	movel	pc@(SysBase),a6
	jsr	a6@(-?)
	bras	.end
.mask	movel	d0,mask

.end	"
	:
	:
	: "d2" "d3" "d4" "d5" "d6" "d7" "a2" "a3" "a4" "a5" "a6" "cc" "memory")
};
