;;; Shows how to implement some commonly used functions for handling lists.

;; determines the length of a list
;; compare this function to the recursive definition of a list.
(define (my-length alist)
  (cond ((null? alist)
	 0)
	(else
	 (+ 1 (my-length (cdr alist))))))

;; creates a copy of a pair (i.e. a cons cell)
(define (pair-copy apair)
  (cons (car apair) (cdr apair)))

;; creates a deep-copy of a tree without checking for for cycles.
;; if the tree to be copied is cyclic, this function will recurse endlessly.
;; note that the leafs of the original tree and its copy will be shared!
;; compare this function to the recursive definition of a binary tree.
(define (pair-tree-deep-copy thing)
  (if (not (pair? thing))
      thing
      (cons (pair-tree-deep-copy (car thing))
	    (pair-tree-deep-copy (cdr thing)))))

;; copies the spine of a list
(define (my-list-copy alist)
  (if (null? alist)
      '()
      (cons (car alist) (my-list-copy (cdr alist)))))

;; append two lists. note that the second list is shared among the original list and its
;; copy
(define (my-append2 list1 list2)
  (cond ((null? list1) list2)
	(cons (car list1) (my-append2 (cdr list2) list2))))

;; reverse a list
(define (my-reverse alist)
  (if (null? alist)
      '()
      (append (reverse (cdr alist))
	      (list (car alist)))))

