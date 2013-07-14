;;; Using closures to implement a simple counter (object)

; Since no one else has access to count it becomes private to the closure and
; can be modified and used within its context. In this particular case, count
; is incremented and its value returned when the procedure returned from
; make-counter is called.

(define (make-counter)
  (let ((count 0))
    (lambda ()
      (set! count (+ count 1))
        count)))


