;
; Some examples of using implication links for pattern matching.
; These examples make extensive use of the code in this directory,
; and can be used for ad-hoc command-line testing.
;
;
;
(define human-implies-animal
	(VariableScopeLink
   	(VariableNode "$H")
   	(ImplicationLink
      	(InheritanceLink
         	(VariableNode "$H")
         	(ConceptNode "human")
			)
    		(InheritanceLink
     	 		(VariableNode "$H")
         	(ConceptNode "animal")
			)
		)
	)
)

(InheritanceLink (stv 1 1)
	(ConceptNode "Linas")
	(ConceptNode "human")
)


(do-varscope human-implies-animal)

(InheritanceLink (stv 1.0 1.0) (cog-new-av 1 0 #f)
    (DefinedFrameNode "#Ingestion")
    (DefinedFrameNode "#Manipulation")
)
(InheritanceLink (stv 1.0 1.0) (cog-new-av 1 0 #f)
    (PredicateNode "grab@a6460c2d-b5f8-4287-8882-028d12de42d2_Manipulation")
    (DefinedFrameNode "#Manipulation")
)
(FrameElementLink (stv 1.0 1.0) (cog-new-av 1 0 #f)
   (DefinedFrameNode "#Manipulation")
   (DefinedFrameElementNode "#Manipulation:Agent")
)


;; Example of an implication with just one predicate
(define v
   (VariableScopeLink
      (VariableNode "$predicateNode")
      (ImplicationLink
         (InheritanceLink
            (VariableNode "$predicateNode")
            (DefinedFrameNode "#Manipulation")
         )
         (VariableNode "$predicateNode")
      )
   )
)

(do-varscope v)

(define v2
   (VariableScopeLink
      (TypedVariableLink
         (VariableNode "$prNode")
         (VariableTypeNode "PredicateNode")
      )
      (ImplicationLink
         (InheritanceLink
            (VariableNode "$prNode")
            (DefinedFrameNode "#Manipulation")
         )
         (VariableNode "$prNode")
      )
   )
)

(do-varscope v2)

(define x
  (ImplicationLink
    (InheritanceLink
      (VariableNode "$predicateNode")
      (DefinedFrameNode "#Manipulation")
    )
    (VariableNode "$predicateNode")
  )
)

(do-implication x)


;; Example of implication with a constant term in it.
(define y
  (ImplicationLink
     (AndLink
       (InheritanceLink
         (PredicateNode "grab@a6460c2d-b5f8-4287-8882-028d12de42d2_Manipulation")
         (DefinedFrameNode "#Manipulation")
       )
       (InheritanceLink
         (VariableNode "$predicateNode")
         (DefinedFrameNode "#Manipulation")
       )
     )
     (VariableNode "$predicateNode")
  )
)


(define w
   (ImplicationLink
      (AndLink
         (InheritanceLink
            (VariableNode "$framePredicateNode")
            (DefinedFrameNode "#Manipulation")
         )
         (InheritanceLink
            (VariableNode "$frameAgentPredicateNode")
            (DefinedFrameElementNode "#Manipulation:Agent")
         )
        
         (FrameElementLink
            (VariableNode "$framePredicateNode")
            (VariableNode "$frameAgentPredicateNode")
         )
         (FrameElementLink
            (PredicateNode "grab@a6460c2d-b5f8-4287-8882-028d12de42d2_Manipulation")
            (VariableNode "$frameAgentPredicateNode")
         )
      )
      (VariableNode "$frameAgentPredicateNode")
   )
)
