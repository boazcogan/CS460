(define (funky v ls)
	(cond 	((<= v 10)
			(if (> v 5)
				(* (+ v) (- v))
				(round (/ v (modulo v 3)))
			)) 
		((= v 11) (cons v ls))
		((< v 13) (car ls))
		((or (number? v) (list? ls) (zero? v) (string? v)) v)
		(else (cond ((null? ls) "empty")))
	)
)

(define (main)
	(if (and (>= 7 5) (not (>= 5 7)))
		(display (funky 5 '(a 5 "hello world" cons if display
			    newline cadr and or not define 
			    number? list? zero? null?
			    string? + - / * modulo round = > < >=
			    <= 'cond else)))
	)
	(newline)
)

(main)

