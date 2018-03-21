ff a 1234
ff b 1000
def c a / b

var d 1
var i 1001

repeat 234 {
	def d d * i
	inc i
}

shrink

cmp c == d
ls-dec
