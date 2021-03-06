'----------------------------------------------
' ReflectedText.vb (c) 2002 by Charles Petzold
'----------------------------------------------
Imports System
Imports System.Drawing
Imports System.Drawing.Drawing2D
Imports System.Windows.Forms

Class ReflectedText
    Inherits FontMenuForm

    Shared Shadows Sub Main()
        Application.Run(new ReflectedText())
    End Sub

    Sub New()
        Text = "Reflected Text"
        Width *= 2
        strText = "Reflect"
        fnt = New Font("Times New Roman", 54)
    End Sub

    Protected Overrides Sub DoPage(ByVal grfx As Graphics, _
            ByVal clr As Color, ByVal cx As Integer, ByVal cy As Integer)
        Dim br As New SolidBrush(clr)
        Dim fAscent As Single = GetAscent(grfx, fnt)
        Dim strfmt As StringFormat = StringFormat.GenericTypographic
        Dim i As Integer

        grfx.TranslateTransform(cx \ 2, cy \ 2)

        For i = 0 To 3
            Dim grfxstate As GraphicsState = grfx.Save()

            Select Case (i)
                Case 0 : grfx.ScaleTransform(1, 1)
                Case 1 : grfx.ScaleTransform(1, -1)
                Case 2 : grfx.ScaleTransform(-1, 1)
                Case 3 : grfx.ScaleTransform(-1, -1)
            End Select

            grfx.DrawString(strText, fnt, br, 0, -fAscent, strfmt)
            grfx.Restore(grfxstate)
        Next i
    End Sub
End Class
