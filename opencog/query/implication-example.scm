scm
;
; implication-example.scm
;
; Linas Vepstas January 2009
;
; This file contains a simple, visual-inspection demo example for the 
; use of the query engine in the handling of ImplicationLinks. The below 
; defines a single implication link and two chunks of data, and then calls
; the implication engine. 
;
; This example can be loaded by cut-n-paste, or by issuing:
;    cat implication-example.scm | telnet localhost 17001
;
; The expected result, after running the below, is that the following
; should be printed:
;
; guile> (do-implication x)
; (ListLink (EvaluationLink (PredicateNode "make_from")
;     (ListLink (ConceptNode "pottery")
;            (ConceptNode "clay"))))
;
; Note the outermost ListLink is simply an enumeration of all of the 
; possible results from the implication; in this case, there is only one
; possible result.
;
(define x
	(ImplicationLink 
		(AndLink
			(EvaluationLink
				(PredicateNode "_obj")
				(ListLink
					(ConceptNode "make")
					(VariableNode "$var0")
				)
			)
			(EvaluationLink
				(PredicateNode "from")
				(ListLink
					(ConceptNode "make")
					(VariableNode "$var1")
				)
			)
		)
		(EvaluationLink
			(PredicateNode "make_from")
			(ListLink
				(VariableNode "$var0")
				(VariableNode "$var1")
			)
		)
	)
)

(define o
	(EvaluationLink (stv 1.0 1.0)
		(PredicateNode "_obj")
		(ListLink
			(ConceptNode "make")
			(ConceptNode "pottery")
		)
	)
)
(define f
	(EvaluationLink  (stv 1.0 1.0)
		(PredicateNode "from")
		(ListLink
			(ConceptNode "make")
			(ConceptNode "clay")
		)
	)
)

(do-implication x)
