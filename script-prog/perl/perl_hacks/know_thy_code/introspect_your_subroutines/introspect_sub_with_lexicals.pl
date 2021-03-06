use Data::Dumper;

package Foo;

sub foo
{
    my ($foo, $bar, $baz) = @_;
}

package Bar;

sub bar {}
*foo = \&Foo::foo;

package main;

warn introspect_sub( \&Foo::foo );
warn introspect_sub( \&Bar::bar );
warn introspect_sub( \&Bar::foo );
warn introspect_sub( \&Dumper );

# introspect_sub() as modified...
