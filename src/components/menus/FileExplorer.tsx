import { Sidebar, MenuItem, Icon, Menu, MenuHeader } from 'semantic-ui-react'
import { useState } from 'react';
import { useAppSelector, useAppDispatch } from '../../hooks';
import type { FileNode } from '../../store/fileSlice'; 
import RecursiveMenuItem from './RecursiveMenuItem';

export default function FileExplorer(
  { fileExplorerOpen}: { fileExplorerOpen: boolean }
) {
  const files: FileNode[] = useAppSelector((state) => state.file.files);

  if (!fileExplorerOpen) {
    return null;
  }

  return (
    <div>
      <Sidebar
        as={Menu}
        animation="push"
        icon="labeled"
        inverted
        vertical
        visible={fileExplorerOpen}
        style={{ maxHeight: '100vh', overflowY: 'auto' }}
      >
        { files.map((file, index) => {
          return (
            <RecursiveMenuItem fileNode={file} />
          );
        })}

        { !files || files.length === 0 && (
          <MenuItem
            name='no files'
            active={false}
            inverted
            style={{ display: 'flex', alignItems: 'baseline', width: '100%' }}
          >
            No files, try opening a project
          </MenuItem>
        )}
      </Sidebar>
    </div>
  );
}
