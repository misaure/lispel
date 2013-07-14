;;;
;;; A small LispScript test program for testing the scanner
;;;

;; some stupid function definition
(define (myfunc1 a b c)
  (begin
    (write a)
    (write b)
    (write c)))

;; check recognition of quote characters
(myfunc2 'hello 'strange 'world)

;; check if boolean literals are recognized
(define false #f)
(define true #t)

;; what happens with special characters like '*'?
(let ((aNumber 100))
  (setf newNumber (* aNumber aNumber)))

