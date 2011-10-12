#!/usr/bin/perl
use warnings;
use strict;
use File::Path;
use File::Copy;

my $NAME = "sudoku-solver";

sub usage {
die<<EOT;
Will drop the released tarballs to the specified directory.
$0 usage:
	$0 majorversion directory 

EOT
}

my $VER = shift @ARGV || usage;
my $dir = shift @ARGV || usage;
die "$dir: not a directory" if ! -d $dir;
die "$dir: not a writable directory" if ! -w $dir;

system("svn log  | tools/gnuify-changelog.pl > Changelog") == 0 or die;
open(PIPE,'-|',"svn info") or die;
my ($url,$rev);
while(<PIPE>) {
	if(m/^URL: (.*)$/) {
		$url = $1;	
	} elsif (m/^Revision: (\d+)$/) {
		$rev = $1;
	}
}
close PIPE;
#copy("README",$dir) or die "copy";
copy("Changelog",$dir) or die "copy";
chdir $dir or die "chdir";
print "url $url  rev $rev dir: $dir\n";
die "url or rev" if ( ! $url || ! $rev);
system("svn export $url $NAME-$VER-rev".$rev) == 0 or die;
system("tar cjf $NAME-$VER-rev".$rev.".tar.bz2 $NAME-$VER-rev".$rev) == 0 or die;
system("tar czf $NAME-$VER-rev".$rev.".tar.gz $NAME-$VER-rev".$rev) == 0 or die;
rmtree(["$NAME-$VER-rev".$rev],0,0);
$_ = "$NAME-LATEST.tar.gz";
unlink $_ if -e $_;
link "$NAME-$VER-rev".$rev.".tar.gz",$_ or die "link";
$_ = "$NAME-LATEST.tar.bz2";
unlink $_ if -e $_;
link "$NAME-$VER-rev".$rev.".tar.bz2",$_ or die "link";
my @latest = <LATEST-IS-*>;
unlink @latest;
system("touch","LATEST-IS-$VER");



