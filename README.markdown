Geanyuniq
=========
Geanyuniq is a plugin for the [Geany](http://geany.org/) lightweight IDE. It adds a command to automatically delete duplicate lines, leaving only the first line in a sequence of duplicates.

For example, consider a file containing this:

    foo
    bar
    bar
    bar
    baz
    baz
    foo

After running Geanyuniq on this document, the contents would be:

    foo
    bar
    baz
    foo


##Installation##
At the moment, Geanyuniq is not available in any pre-packaged form, so you need to build it yourself. Luckily, that isn't too hard. The following steps should do it:

1. Grab the code (`git clone git://github.com/unwind/geanyuniq.git`)
2. Using a terminal, enter the top-level `geanyuniq/` directory
3. Type `make`
4. Type `sudo ln -s $(pwd)/src/geanyuniq.so $(dirname $(dirname $(which geany)))/lib/geany`

The final step is the most complicated; the purpose is to create a symbolic link from Geany's system-wide plugin directory to the library you just built. This assumes that you will want to keep the plugin in the source directory; if you rather would install it permanently you can move it to the indicated location instead, and then delete the source code.

Keeping a symbolic link will of course make it easier to update the plugin if there are any changes (or if you do changes yourself).


#Running Geanyuniq#
By default, Geanyuniq does not assigns itself a keyboard shortcut, but you can easily do this through Geany's main Preferences window.
You can also find a menu item labelled "Delete Duplicate Lines" in the "Tools" menu.

If you run the command with a selection active, duplicate lines will only be deleted in the selection. Geanyuniq will consider the line that the selection starts
on to be part of the selection, even if the selection starts somewhere in the middle of the line. It will also consider the line that the selection ends on to be
the last line of the selection, i.e. the entire line might be removed if it's the same as the one before it.


##Configuring Geanyuniq##
Currently, Geanyuniq has no configuration options.


#Feedback#
Please contact the author, Emil Brink (by e-mailing &lt;emil@obsession.se&gt;) regarding any bugs, comments, or thoughts about Geanyuniq. Enjoy.
