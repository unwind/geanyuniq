Geanyuniq
=========
Geanyuniq is a plugin for the [Geany](http://geany.org/) lightweight IDE. It adds two commands to automatically delete duplicate lines:

1. Delete Adjacent Duplicate Lines, which will leave only the first line in a sequence of duplicates.
2. Delete All Duplicate Lines, which removes all duplicates regardless of how they are arranged.

For example, consider a file containing this:

    foo
    bar
    bar
    bar
    baz
    baz
    foo

After running "Delete Adjacent Duplicate Lines" on this document, the contents would be:

    foo
    bar
    baz
    foo

**NOTE:** There are still two occurances of the line whose content is just `foo`; Geanyuniq will only remove duplicate lines that are immediately next to each other.

However, after running "Delete All Duplicate Lines", the result is:

    foo
    bar
    baz

Notice how this removes all duplicates.

## Installation ##
At the moment, Geanyuniq is not available in any pre-packaged form, so you need to build it yourself. Luckily, that isn't too hard. The following steps should do it:

1. Grab the code (`git clone git://github.com/unwind/geanyuniq.git`)
2. Using a terminal, enter the top-level `geanyuniq/` directory
3. Type `make`
4. There are two options here. If you just want to *use* the version of Geanyuniq you just built, and not keep updating it if the source changes, type `sudo make install`. If you want to keep the source (for updates, hacking, whatever), type `sudo make install-dev`.
5. If you went with the `make install` option, you can now delete the directory holding the source code.

# Running Geanyuniq #
By default, Geanyuniq does not assigns itself any keyboard shortcuts, but you can easily do this through Geany's main Preferences window.

So, to invoke Geanyuniq, use the two new menu items in the "Tools" menu, labelled "Delete Adjacent Duplicate Lines" and
"Delete All Duplicate Lines".

## Selection Handling ##
Either command will, when run, check for a selection and if one exists duplicates will only be deleted *inside* the selection.

Geanyuniq will consider the line that the selection starts on to be part of the selection, even if the selection starts somewhere in the middle of the line.
It will also consider the line that the selection ends on to be the last line of the selection, i.e.
the entire line might be removed if it's the same as the one before it.

## Configuring Geanyuniq ##
Currently, Geanyuniq has no configuration options.

# Feedback #
Please contact the author, Emil Brink (by e-mailing &lt;emil@obsession.se&gt;) regarding any bugs, comments, or thoughts about Geanyuniq. Enjoy.
