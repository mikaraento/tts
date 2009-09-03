Some notes on the TTS Proto FEP

Getting contents of on-screen controls on Symbian is not possible outside the
process that owns the controls. Hence a FEP is used - FEPs are loaded into all
processes (threads) that use CONE (CCoeEnv).

In production mode the goals for this code are:
   - recognize the current application view
   - retrieve known currently visible controls (lists, text fields)
   - retrieve the interesting content/state of the known controls
   - recognize menus/popups on screen and get their content
   - translate all the content into a common format

The challenge is in the 'known' control part: how do we get access to the
relevant controls and know what classes they belong to. In development mode the
goals for this code are:
   - get access to all possible controls in the current process
   - try to recognize automatically what class they are derived from
   - dump the control tree with all possible information
   - manually inspect the dump to learn how the control tree is structured for
     each application.

Accessing the control tree requires digging around the internal state of the
various UI objects. For this purpose some headers have been copied to
modified_system_include and all the protected and private fields declared as
public.
