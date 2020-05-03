sub uniq{
	my @arr = @_;
	my %hash;
	@hash(@arr) = ();
	return keys %hash;
}
