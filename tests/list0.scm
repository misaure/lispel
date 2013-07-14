;;
;; test list processing builtins
;;

(define mycons (cons "hello" (quote ())))
(define mycon2 (cons "world" (quote ())))

(define thelist (cons "hello" (cons "world" (quote ()))))

(set-cdr! mycons mycon2) ;; should return ("hello" "world")

(if (equal? mycons thelist)
  (display "SUCCESS")
  (display "FAILURE"))

