;; test let

(define x 1)
(define y 2)

(let ((x 5) (y x))
   (+ x y))
; ==> 6

(let* ((x 5) (y x))
   (+ x y))
; ==> 10

