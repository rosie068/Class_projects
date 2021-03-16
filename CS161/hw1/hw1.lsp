;;; CS 161 hw 1
;;; Rosemary He
;;; 204-753-981

;;; PAD calculates the nth element in the Padovan sequence
;;; N @param is the nth element
(defun PAD (N) ;1 Padovan sequence
    (if (< N 3) 1 ;if n = 0,1,2, return 1
        (+ (PAD(- N 2)) (PAD(- N 3)))
    )
)

;;; PADOUT prints out the first n test cases in PAD
;;; N @param number of test cases
(defun PADOUT (N)
    (loop for x from 0 to (+ N 1)
        for y = (PAD x)
        collect y
    )
)

;;; SUMS calculates the # of additions needed to find the nth element in PAD
;;; N @param is the nth element
(defun SUMS (N) ;sum calculates # additions required by PAD
    (if (< N 3) 0 ;if n = 0,1,2, no addition required
        (+ 1 (SUMS(- N 2)) (SUMS(- N 3))) ;else ite = ite(n-2) + ite(n-3) + 1
    )
)

;;; SUMSOUT prints the first n test cases in SUMS
;;; N @param number of test cases
(defun SUMSOUT (N)
    (loop for x from 0 to (+ N 1)
        for y = (SUMS x)
        collect y
    )
)

;;; ANON changes all values in a tree to ?, but keeps the same structure
;;; TREE @param the tree object to be changed
(defun ANON (TREE)
    (cond ((atom TREE) '?) ;atom, return ?
        ((= (length TREE) 1) '?) ;if list has only 1 element, return ?
        ((= (length TREE) 2) (list (ANON (first TREE)) (ANON(second TREE)) ))
        (t (cons (ANON (car TREE)) (ANON (cdr TREE))) )
    )
)